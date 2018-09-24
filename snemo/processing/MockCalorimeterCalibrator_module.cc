/// Mock Calorimeter Calibrator as an Art EDProducer
// Consumes: StepHitCollections for calo, xcalo
// Produces: CalorimeterHitCollection

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

namespace snemo {
class MockCalorimeterCalibrator : public art::EDProducer {
 public:
  MockCalorimeterCalibrator(fhicl::ParameterSet const& ps);

  void produce(art::Event& e) override;
};


MockCalorimeterCalibrator::MockCalorimeterCalibrator(fhicl::ParameterSet const&) {
}

void
MockCalorimeterCalibrator::produce(art::Event&) {
  // Consume products here...

  // Call the processing external function/class here...

  // Produce products here...
}

} // namespace snemo

DEFINE_ART_MODULE(snemo::MockCalorimeterCalibrator)
