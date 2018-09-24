/// Mock Geiger Calibrator as an Art EDProducer
// Consumes: StepHitCollections for calo, xcalo
// Produces: GeigerHitCollection

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

namespace snemo {
class MockGeigerCalibrator : public art::EDProducer {
 public:
  MockGeigerCalibrator(fhicl::ParameterSet const& ps);

  void produce(art::Event& e) override;
};


MockGeigerCalibrator::MockGeigerCalibrator(fhicl::ParameterSet const&) {
}

void
MockGeigerCalibrator::produce(art::Event&) {
  // Consume products here...

  // Call the processing external function/class here...

  // Produce products here...
}

} // namespace snemo

DEFINE_ART_MODULE(snemo::MockGeigerCalibrator)
