/// Example EDAnalyzer module using TFileService

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

// TFileService and associated ROOT interfaces
#include "art/Framework/Services/Optional/TFileService.h"
#include "TH1F.h"

#include "fhiclcpp/ParameterSet.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

namespace snemo {
  /// Trivial analyzer module
  class FLExampleAnalyzer : public art::EDAnalyzer {
  public:
    explicit FLExampleAnalyzer(fhicl::ParameterSet const& ps);

    /// per-Job config
    void beginJob() override;

    /// Analyze the event: NB: const, so read-only!
    void analyze(art::Event const& event) override;

  private:
    TH1F* myHist_{nullptr}; //< Pointer to histogram created by TFS
  };

  // Implementation
  FLExampleAnalyzer::FLExampleAnalyzer(fhicl::ParameterSet const& ps)
    : art::EDAnalyzer{ps}
  {
    // We should declare what we consume here
  }

  void
  FLExampleAnalyzer::beginJob()
  {
    // Here we only analyze over the Job, which may be anything from
    // a partial SubRun up to multiple full/partial Runs.
    // If we want to split plots etc by Run/SubRun into directories or
    // separate files, we need something more involved.
    art::ServiceHandle<art::TFileService> tfs;

    // Put our histogram into a directory
    // Note that Art automatically puts all ROOT products
    // of this module into a TDirectory with name equal
    // to the module label
    art::TFileDirectory myDir = tfs->mkdir("FLExampleAnalyzer");
    myHist_ = myDir.make<TH1F>("Sum", "Sum of two products", 21, -10., 10.);

    mf::LogDebug("example") << "Constructed myHist_(" << myHist_ << ")";
  }

  void
  FLExampleAnalyzer::analyze(art::Event const& event)
  {
    mf::LogDebug("example") << "Analyzing event: " << event.id();
    myHist_->Fill(1.0);
  }

} // namespace snemo

// Declare the class to Art
DEFINE_ART_MODULE(snemo::FLExampleAnalyzer)
