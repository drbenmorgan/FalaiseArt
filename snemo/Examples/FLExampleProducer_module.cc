/// Example Art EDProducer

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "fhiclcpp/ParameterSet.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include <iostream>
#include <memory>
#include <string>

namespace snemo {
  /// Simple producer of integers
  class FLExampleProducer : public art::EDProducer {
  public:
    // Required constructor, this passes the configuration
    explicit FLExampleProducer(fhicl::ParameterSet const& ps);

    // Process the event
    void produce(art::Event& e) override;

  private:
    std::string message_; //< A message to be logged
    int value_;           //< The value to be put in the event
  };

  // Implementation
  FLExampleProducer::FLExampleProducer(fhicl::ParameterSet const& ps)
    : message_{ps.get<std::string>("message", "Hello world")}
    ,                            // For the message, we have a default
    value_{ps.get<int>("value")} // For the value, we leave out the default, Art
                                 // will throw unless we supply it
  {
    // This declares the module to produce an int in an Event, this
    // is for internal Art bookkeeping, and as we'll see,
    // how we can later retrieve this value in other modules
    // No additional arguments in "()" - default is event level
    // and don't have clashing names. If we have two products of
    // the same type, need to use instance names to distinguish
    produces<int>();
  }

  void
  FLExampleProducer::produce(art::Event& e)
  {
    // This should only be printed at debug level, or when we select the
    // "example" category.
    mf::LogDebug("example") << message_;

    // make and put the value into the event
    auto p = std::make_unique<int>(value_);
    e.put(std::move(p));
  }

} // namespace snemo

// Declare the class to Art
DEFINE_ART_MODULE(snemo::FLExampleProducer)
