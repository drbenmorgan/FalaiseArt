/// Mock Calorimeter Calibrator as an Art EDProducer
// Consumes: StepHitCollections for calo, xcalo
// Produces: CalorimeterHitCollection

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "snemo/datamodel/CalorimeterHitCollection.h"
#include "snemo/datamodel/StepHitCollection.h"
#include "snemo/processing/MockCalorimeterModel.h"

#include <algorithm>

namespace {
  using namespace fhicl;

  struct CaloModelConfig {
    Atom<double> energyResolution{Name("energyResolution"),
                                  Comment("Calorimeter energy resolution in % for e- at 1MeV")};

    Sequence<double, 3> alphaQuenchingParameters{
      Name("alphaQuenchingParameters"),
      Comment("Dimensionless parameters for the Alpha Quenching function")};

    Atom<double> relaxationTime{Name("relaxationTime"),
                                Comment("Scintillator relaxation time in ns")};

    explicit operator snemo::MockCalorimeterModel() const
    {
      // Make some assumptions, temporarily, about units.
      // TODO: define proper FHiCL bindings for quantities that Bayeux would
      // recognize, converting them to plain doubles here
      return snemo::MockCalorimeterModel{energyResolution() * CLHEP::perCent,
                                         alphaQuenchingParameters(),
                                         relaxationTime() * CLHEP::ns};
    }
  };

  struct MCCConfig {
    Atom<std::string> tagLabel{
      Name("tagLabel"),
      Comment("Label of StepHitCollection product to consume, e.g. SD:calo")};

    Atom<double> lowEnergyThreshold{Name("lowEnergyThreshold"), Comment("Low threshold in keV")};

    Atom<double> highEnergyThreshold{Name("highEnergyThreshold"), Comment("High threshold in keV")};

    Atom<double> timeWindow{Name("timeWindow"),
                            Comment("Time in ns beyond which hits will be discarded")};

    Table<CaloModelConfig> caloModel{Name("caloModel"),
                                     Comment("Parameters for modelling the calorimeter")};
  };
} // namespace

namespace snemo {
  class MockCalorimeterCalibrator : public art::EDProducer {
  public:
    using Parameters = art::EDProducer::Table<MCCConfig>;
    explicit MockCalorimeterCalibrator(Parameters const& ps);

    void produce(art::Event& e) override;

  private:
    std::unique_ptr<CalorimeterHitCollection> calibrate(StepHitCollection const&,
                                                        MockCalorimeterModel const&);

    art::ProductToken<StepHitCollection> caloHitsToken_;
    art::RandomNumberGenerator::base_engine_t& rngEngine_;
    double lowEnergyThreshold_;
    double highEnergyThreshold_;
    double timeWindow_;
    MockCalorimeterModel caloModel_;
  };

  MockCalorimeterCalibrator::MockCalorimeterCalibrator(Parameters const& ps)
    : caloHitsToken_{consumes<StepHitCollection>(ps().tagLabel())}
    , rngEngine_{createEngine(-1)}
    , lowEnergyThreshold_{ps().lowEnergyThreshold() * CLHEP::keV}
    , highEnergyThreshold_{ps().highEnergyThreshold() * CLHEP::keV}
    , timeWindow_{ps().timeWindow() * CLHEP::ns}
    , caloModel_{ps().caloModel()}
  {
    produces<CalorimeterHitCollection>();
  }

  void
  MockCalorimeterCalibrator::produce(art::Event& e)
  {
    // Consume products here...
    auto caloHits = e.getValidHandle(caloHitsToken_);

    // Produce outputs via canonical form. Whilst the
    // call is to a member function, it could equally be to
    // a separate function, keeping framework and algorithm separate
    e.put(this->calibrate(*caloHits, caloModel_));
  }

  std::unique_ptr<CalorimeterHitCollection>
  MockCalorimeterCalibrator::calibrate(StepHitCollection const& rawHits,
                                       MockCalorimeterModel const& calModel)
  {
    auto outputHits = std::make_unique<CalorimeterHitCollection>();

    // 1. "Digitization" of step hits
    for (const auto& stepHit : rawHits) {
      // auto hitID = stepHit.get_hit_id();
      auto startTime = stepHit.get_time_start();
      auto energyDep = stepHit.get_energy_deposit();
      auto geomId = stepHit.get_geom_id();

      if (stepHit.get_particle_name() == "alpha") {
        energyDep = calModel.quenchAlphaEnergy(energyDep);
      }

      // Check if there have already been steps in this calorimeter
      // We reverse search as stepHits should generally bunch same
      // geometry ids together
      auto foundHit =
        std::find_if(outputHits->rbegin(), outputHits->rend(), [&geomId](CalorimeterHit const& x) {
          return x.geoID == geomId;
        });

      if (foundHit == outputHits->rend()) {
        // Then it's a new hit
        outputHits->emplace_back(
          CalorimeterHit{outputHits->size(), geomId, energyDep, 0.0, startTime, 0.0});
      } else {
        // Hit already, update as required
        // 1. Timing
        double deltaTime = startTime - foundHit->time;

        // Drop new hit as it arrives too late
        if (deltaTime > timeWindow_) {
          continue;
        }

        // Drop existing hit as it arrives too late after the new one
        if (deltaTime < -timeWindow_) {
          foundHit->time = startTime;
          foundHit->energy = energyDep;
          continue;
        }

        // 2. Arrival time, total energy
        foundHit->time = std::min(startTime, foundHit->time);
        foundHit->energy += energyDep;
      }
    }

    // 2. "Calibration" from "Digitized" hits
    for (auto& caloHit : *outputHits) {
      // NB, calls are order dependent: Use smeared energies to recalculate
      // errors
      caloHit.energy = calModel.smearEnergy(caloHit.energy, rngEngine_);
      caloHit.energyError = calModel.getEnergySigma(caloHit.energy);

      caloHit.time = calModel.smearTime(caloHit.time, caloHit.energy, rngEngine_);
      caloHit.timeError = calModel.getTimeSigma(caloHit.energy);
    }

    // 3. "Triggering" (basically thresholding based on the high/low
    // values)
    // - If collection has 1 or more hits above high_threshold
    //   - remove any hits below low_threshold
    // - Else remove all hits
    auto hitAboveHighThreshold = [thresh = this->highEnergyThreshold_](CalorimeterHit const& h)
    {
      return h.energy >= thresh;
    };

    if (std::any_of(outputHits->begin(), outputHits->end(), hitAboveHighThreshold)) {
      auto hitBelowLowThreshold = [thresh = this->lowEnergyThreshold_](CalorimeterHit const& h)
      {
        return h.energy < thresh;
      };

      auto eraseStartIter =
        std::remove_if(outputHits->begin(), outputHits->end(), hitBelowLowThreshold);
      outputHits->erase(eraseStartIter, outputHits->end());
    } else {
      outputHits->clear();
    }

    return outputHits;
  }

} // namespace snemo

DEFINE_ART_MODULE(snemo::MockCalorimeterCalibrator)
