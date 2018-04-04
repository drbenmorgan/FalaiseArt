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

// FHiCL configuration for this module. Like the module,
// NOT a public interface, so in anon NS per Art recommendations
namespace {
  using namespace fhicl;

  struct FLECPConfig {
    // FHiCL convention/recommendation is to have instance
    // names == Name("<string>") for clarity. Name() is the
    // FHiCL parameter identifier the client will use, whereas
    // we use the instance name to retrieve values.
    Atom<std::string> message{Name("message"), // This is the id of the parameter in FHiCL
                              Comment("message to be logged"), // Help on this parameter
                              "Hello World"}; // Default value (see also OptionalAtom<T>)
    Atom<int> value{Name("value"),
                    Comment("value of product to store in event")};
  };
} // namespace

namespace snemo {
  /// Simple producer of integers, plus config validation
  class FLExampleConfigurableProducer : public art::EDProducer {
  public:
    // This type alias enables printout of the description of the config struct
    // via `art --print-description FLExampleConfigurableProducer`
    // We wrap our config struct in that of EDProducer as ours
    // extends that.
    using Parameters = art::EDProducer::Table<FLECPConfig>;

  public:
    // Required constructor, this passes the configuration
    explicit FLExampleConfigurableProducer(Parameters const& ps);

    // Process the event
    void produce(art::Event& e) override;

  private:
    std::string message_; //< A message to be logged
    int value_;           //< The value to be put in the event
  };

  // Implementation
  FLExampleConfigurableProducer::FLExampleConfigurableProducer(
    Parameters const& ps)
    : message_{ps().message()}, // Retrieve parameter via function calls, FHiCL handles the rest
      value_{ps().value()}
  {
    // This declares the module to produce an int, this
    // is for internal Art bookkeeping, and as we'll see,
    // how we can later retrieve this value in other modules
    produces<int>();
  }

  void
  FLExampleConfigurableProducer::produce(art::Event& e)
  {
    // This should only be printed at debug level
    mf::LogDebug("example") << message_;

    // make and put the value into the event
    auto p = std::make_unique<int>(value_);
    e.put(std::move(p));
  }

} // namespace snemo

// Declare the class to Art
DEFINE_ART_MODULE(snemo::FLExampleConfigurableProducer)
