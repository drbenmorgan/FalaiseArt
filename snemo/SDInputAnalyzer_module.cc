/// Simple Analyzer to check we can read input
/// from "SD" bank

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

#include "bayeux/geomtools/clhep.h"
#include "snemo/datamodel/GenBBPrimaryEvent.h"
#include "snemo/datamodel/StepHitCollection.h"

namespace snemo {
  class SDInputAnalyzer : public art::EDAnalyzer {
  public:
    explicit SDInputAnalyzer(fhicl::ParameterSet const& ps);

    void analyze(art::Event const& event) override;

  private:
    // Hold input and product tags
    art::InputTag const sdTag_;
    art::ProductToken<geomtools::vector_3d> const vertexToken_;
    art::ProductToken<snemo::GenBBPrimaryEvent> const mcEventToken_;
    art::ProductToken<snemo::StepHitCollection> const ggHitsToken_;
    art::ProductToken<snemo::StepHitCollection> const caloHitsToken_;
    art::ProductToken<snemo::StepHitCollection> const xcaloHitsToken_;
  };

  SDInputAnalyzer::SDInputAnalyzer(fhicl::ParameterSet const& ps)
    : art::EDAnalyzer(ps)
    , sdTag_{"SD"}
    , vertexToken_{consumes<geomtools::vector_3d>(sdTag_)}
    , mcEventToken_{consumes<snemo::GenBBPrimaryEvent>(sdTag_)}
    , ggHitsToken_{consumes<snemo::StepHitCollection>(art::InputTag{sdTag_.label(), "gg"})}
    , caloHitsToken_{consumes<snemo::StepHitCollection>(art::InputTag{sdTag_.label(), "calo"})}
    , xcaloHitsToken_{consumes<snemo::StepHitCollection>(art::InputTag{sdTag_.label(), "xcalo"})}
  {}

  void
  SDInputAnalyzer::analyze(art::Event const& event)
  {
    // Consume products via their tokens
    auto const& primaryVertex = event.getValidHandle(vertexToken_);
    auto const& mcEvent = event.getValidHandle(mcEventToken_);
    auto const& geigerHits = event.getValidHandle(ggHitsToken_);
    auto const& caloHits = event.getValidHandle(caloHitsToken_);
    auto const& xcaloHits = event.getValidHandle(xcaloHitsToken_);

    std::cout << "Event: \n";
    std::cout << "primaryVertex: " << *primaryVertex << std::endl;
    std::cout << "primaryEvent: " << mcEvent->get_label() << std::endl;
    std::cout << "geiger hits: " << geigerHits->size() << "\n";
    std::cout << "calo hits: " << caloHits->size() << "\n";
    std::cout << "xcalo hits: " << xcaloHits->size() << "\n";
  }

} // namespace snemo

DEFINE_ART_MODULE(snemo::SDInputAnalyzer)
