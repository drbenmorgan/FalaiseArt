/// Example input source, see:
/// https://cdcvs.fnal.gov/redmine/projects/art/wiki/Writing_your_own_input_sources
/// for further details

#include "art/Framework/Core/InputSourceMacros.h"
#include "art/Framework/IO/Sources/Source.h"
#include "art/Framework/IO/Sources/SourceTraits.h"

// Can we log?
#include "messagefacility/MessageLogger/MessageLogger.h"

// Forward declare implementation of input source
namespace snemo {
  class FLExampleInputSourceDriver;
}

// Can move this to separate file(s) later as needed
// It's not (AFAIK) a public interface
class snemo::FLExampleInputSourceDriver {
public:
  // Required constructor
  FLExampleInputSourceDriver(fhicl::ParameterSet const&,
                             art::ProductRegistryHelper&,
                             art::SourceHelper const&);

  // Required member function
  void readFile(std::string const& filename, art::FileBlock*& fb);

  // Required member function
  bool readNext(art::RunPrincipal const* const inR,
                art::SubRunPrincipal const* const inSR,
                art::RunPrincipal*& outR,
                art::SubRunPrincipal*& outSR,
                art::EventPrincipal*& outE);

  // Required member function
  void closeCurrentFile();

private:
  art::SourceHelper const&
    srcHelper_;             //< {Run,SubRun,Event}Principal construction helper
  std::string currentFile_; //< Name of file currently being read
  size_t currentEvent_;     //< event counter
};

// Implementation of the driver
snemo::FLExampleInputSourceDriver::FLExampleInputSourceDriver(
  fhicl::ParameterSet const& ps,
  art::ProductRegistryHelper&,
  art::SourceHelper const& aSH)
  : srcHelper_{aSH}, currentFile_{}, currentEvent_{0}
{
  // We don't yet register any products to art::ProductRegistryHelper is
  // unused
}

void
snemo::FLExampleInputSourceDriver::readFile(std::string const& filename,
                                            art::FileBlock*& fb)
{
  mf::LogDebug("BrioInput") << "Reading file: " << filename;
  // Create the file block for the new file
  fb =
    new art::FileBlock{art::FileFormatVersion{1, "BRIOInput 2018.0"}, filename};

  // store current file and initialize event counter
  currentFile_ = filename;
  currentEvent_ = 0;
}

bool
snemo::FLExampleInputSourceDriver::readNext(
  art::RunPrincipal const* const inR,
  art::SubRunPrincipal const* const inSR,
  art::RunPrincipal*& outR,
  art::SubRunPrincipal*& outSR,
  art::EventPrincipal*& outE)
{
  // This largely follows the "GeneratorTestDetail" test in art
  // which is effectively one
  // Always 10 events!
  mf::LogDebug("BrioInput") << "readNext: " << currentEvent_;
  if (++currentEvent_ > 10) {
    return false;
  }

  // If input RunPrincipal is null, we need to create one (we are not yet
  // checking for boundaries)
  if (inR == nullptr) {
    outR = srcHelper_.makeRunPrincipal(1, art::Timestamp{});
  }
  // Same for input SubRunPrincipal
  if (inSR == nullptr) {
    art::SubRunID srID{outR ? outR->run() : inR->run(), 0};
    outSR = srcHelper_.makeSubRunPrincipal(
      srID.run(), srID.subRun(), art::Timestamp{});
  }

  // ALWAYS create outE...
  outE = srcHelper_.makeEventPrincipal(outR ? outR->run() : inR->run(),
                                       outSR ? outSR->subRun() : inSR->subRun(),
                                       currentEvent_,
                                       art::Timestamp{});

  return true;
}

void
snemo::FLExampleInputSourceDriver::closeCurrentFile()
{
  mf::LogDebug("BrioInput") << "Closing file: " << currentFile_;
  currentFile_.clear();
}

// If we need to specify traits specializations, they go here
// e.g. Source_wantFileServices
//      Source_Generator
// Whilst we eventually want to use the file services, it'll (usefully)
// check for file existence, but we're only processing with dummy names.
// So we *don't yet* want file services (later will also want different checks
// on filename searches, e.g. additional search paths and so on)
//
// NB: like other specializations, must be in art's NS:
namespace art {
  template <>
  struct Source_wantFileServices<snemo::FLExampleInputSourceDriver> {
    static constexpr bool value = false;
  };
} // namespace art

// Make the implementation usable in the framework
namespace snemo {
  using FLExampleInputSource = art::Source<FLExampleInputSourceDriver>;
}

DEFINE_ART_INPUT_SOURCE(snemo::FLExampleInputSource)

// Can also do
// DEFINE_ART_INPUT_SOURCE(art::Source<FLExampleInputSourceDriver>);
