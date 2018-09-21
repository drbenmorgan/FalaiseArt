/// Example input source, see:
/// https://cdcvs.fnal.gov/redmine/projects/art/wiki/Writing_your_own_input_sources
/// for further details

#include "art/Framework/Core/InputSourceMacros.h"
#include "art/Framework/IO/Sources/Source.h"
#include "art/Framework/IO/Sources/SourceTraits.h"
#include "art/Framework/IO/Sources/put_product_in_principal.h"

// Can we log?
#include "messagefacility/MessageLogger/MessageLogger.h"

// Falaise/Bayeux
#include "bayeux/brio/reader.h"
#include "bayeux/datatools/things.h"

#include "snemo/datamodel/event_header.h"

// Forward declare implementation of input source
namespace snemo {
  class BrioInputSourceDriver;
}

class snemo::BrioInputSourceDriver {
 public:
  // Required constructor
  BrioInputSourceDriver(fhicl::ParameterSet const&,
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
  // Return true if the supplied reader has required stores
  bool validateSchema(const brio::reader& r) const;

 private:
  art::SourceHelper const&
    srcHelper_;             ///< {Run,SubRun,Event}Principal construction helper
  brio::reader bInput_;     ///< Reader for BRIO files
  std::string const GI_STORE {"GI"};
  std::string const ER_STORE {"ER"};
};

// Implementation of the driver
snemo::BrioInputSourceDriver::BrioInputSourceDriver(
  fhicl::ParameterSet const& /*ps*/,
  art::ProductRegistryHelper& helper,
  art::SourceHelper const& aSH)
  : srcHelper_{aSH}, bInput_{}
{
  // Products this source will reconstitute into Principals
  // string is the module label.
  helper.reconstitutes<int, art::InEvent>("BrioInputSource");
  // Need a dictionary to do the following:
  //helper.reconstitutes<datatools::properties, art::InEvent>("BrioInputSource");
}


void
snemo::BrioInputSourceDriver::readFile(std::string const& filename,
                                            art::FileBlock*& fb)
{
  bInput_.open(filename);
  std::cout << "Opening file " << filename << std::endl;
  if ( !this->validateSchema(bInput_) ) {
    // Need to throw if we get here
    // "throw cet::CodedException("BrioInput")"?
  };

  // Create the file block for the new file
  fb =
    new art::FileBlock{art::FileFormatVersion{1, "BRIOInput 3.3"}, filename};
}

bool
snemo::BrioInputSourceDriver::readNext(
  art::RunPrincipal const* const inR,
  art::SubRunPrincipal const* const inSR,
  art::RunPrincipal*& outR,
  art::SubRunPrincipal*& outSR,
  art::EventPrincipal*& outE)
{
  // BRIO is based on while/next iteration...
  if ( !bInput_.has_next(ER_STORE) ) {
    return false;
  }

  // If input RunPrincipal is null, we need to create one (we are not yet
  // checking for boundaries, don't expect them in Brio files)
  if (inR == nullptr) {
    outR = srcHelper_.makeRunPrincipal(1, art::Timestamp{});
    // GI_STORE likely has run level info, maybe even global (where it would be
    // handled in open/close file and Service attachment
    //datatools::properties p;
    //bInput_.load(p, GI_STORE, 0);
    //p.tree_dump();
    //art::put_product_in_principal(std::make_unique<datatools::properties>(p), *outE, "BrioInputSource");
  }
  // Same for input SubRunPrincipal
  if (inSR == nullptr) {
    art::SubRunID srID{outR ? outR->run() : inR->run(), 0};
    outSR = srcHelper_.makeSubRunPrincipal(
      srID.run(), srID.subRun(), art::Timestamp{});
  }

  // ALWAYS create outE...
  datatools::things e;
  bInput_.load_next(e, ER_STORE);
  auto EH = e.get<snemo::datamodel::event_header>("EH");
  auto eID = EH.get_id().get_event_number();
  EH.tree_dump();

  outE = srcHelper_.makeEventPrincipal(outR ? outR->run() : inR->run(),
                                       outSR ? outSR->subRun() : inSR->subRun(),
                                       eID,
                                       art::Timestamp{});
  art::put_product_in_principal(std::make_unique<int>(eID), *outE, "BrioInputSource");

  return true;
}

void
snemo::BrioInputSourceDriver::closeCurrentFile()
{
  bInput_.close();
}

bool snemo::BrioInputSourceDriver::validateSchema(const brio::reader& /*r*/) const {
  return true;
}

// Make the implementation usable in the framework
namespace snemo {
  using BrioInputSource = art::Source<BrioInputSourceDriver>;
}

DEFINE_ART_INPUT_SOURCE(snemo::BrioInputSource)

