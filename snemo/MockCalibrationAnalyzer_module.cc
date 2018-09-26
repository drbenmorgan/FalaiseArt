/// Simple Analyzer to consume and aggregate
// calorimeter info

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Optional/TFileService.h"

#include "CLHEP/Units/SystemOfUnits.h"
#include "TH1F.h"

#include "snemo/datamodel/CalorimeterHitCollection.h"

namespace snemo {
  class MockCalibrationAnalyzer : public art::EDAnalyzer {
  public:
    explicit MockCalibrationAnalyzer(fhicl::ParameterSet const& ps);

    void analyze(art::Event const& event) override;

  private:
    // Hold input and product tags
    art::InputTag const mainWallTag_;
    art::InputTag const xWallTag_;
    art::ProductToken<snemo::CalorimeterHitCollection> const caloHitsToken_;
    art::ProductToken<snemo::CalorimeterHitCollection> const xcaloHitsToken_;

    // Analysis objects
    TH1F* totalEnergy_;
  };

  MockCalibrationAnalyzer::MockCalibrationAnalyzer(fhicl::ParameterSet const& ps)
    : art::EDAnalyzer(ps)
    , mainWallTag_{"CaloCD"}
    , xWallTag_{"XCaloCD"}
    , caloHitsToken_{consumes<snemo::CalorimeterHitCollection>(mainWallTag_)}
    , xcaloHitsToken_{consumes<snemo::CalorimeterHitCollection>(xWallTag_)}
  {
    // Setup analysis objects. NB, doesn't handle file switching yet
    art::ServiceHandle<art::TFileService> fs;
    totalEnergy_ =
      fs->make<TH1F>("totalEnergy", "Summed energy in all hit calorimeters", 200, 0.0, 2.0);
  }

  void
  MockCalibrationAnalyzer::analyze(art::Event const& event)
  {
    // Consume products via their tokens
    auto const& caloHits = event.getValidHandle(caloHitsToken_);
    auto const& xcaloHits = event.getValidHandle(xcaloHitsToken_);

    // Accumulate energies
    double caloEnergy = std::accumulate(
      caloHits->begin(), caloHits->end(), 0.0, [](double a, CalorimeterHit const& h) {
        return a + h.energy;
      });
    double xwallEnergy = std::accumulate(
      xcaloHits->begin(), xcaloHits->end(), 0.0, [](double a, CalorimeterHit const& h) {
        return a + h.energy;
      });
    // Fill, converting to MeV
    totalEnergy_->Fill((caloEnergy + xwallEnergy) / CLHEP::MeV);
  }

} // namespace snemo

DEFINE_ART_MODULE(snemo::MockCalibrationAnalyzer)
