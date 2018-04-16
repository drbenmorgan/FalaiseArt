// Test portability of datatools/brio files across
// platforms and ROOT/Boost versions, C++ standard
// Canonical case
// 1) Write file with ROOT 6.08, Boost 1.63, C++11
// 2) Read file with ROOT 6.12, Boost 1.66, C++14
//
// Does this work for brio?
// Does this work for xml?
//
// Motivated by not being able to read ER bank from
// current flsimulate files with just Bayeux.
// Properties are o.k., so not clear why things/mctools::simulated_data
// cannot be read (missing Falaise objects, binary incompatibility
// etc)
//
// Create dummy outputs here using pure Bayeux read/write
// Compile/run against standard packaged Bayeux/Root for
// SuperNEMO, then this updated C++14 version. This should
// remove any Falaise dependency.
//
// TODO: Check compatibility of ROOT files/dicts across
// C++ Std versions (e.g. generated file using dict compiled
// against C++11, read with dict compiled against C++14)
//

#include <iostream>
#include <string>

#include "bayeux/brio/writer.h"
#include "bayeux/brio/reader.h"

#include "bayeux/datatools/properties.h"
#include "bayeux/datatools/things.h"

#include "bayeux/mctools/simulated_data.h"

void write(const std::string& path) {
  std::clog << "Writing file `" << path << "`\n";

  // Open (Brio) file
  brio::writer output{path};

  // Add "GI" bank, add datatools::properties entries
  output.add_store("GI", datatools::properties::SERIAL_TAG);

  // Add "ER" bank, add mctools::simulated_data entries
  output.add_store("ER", datatools::things::SERIAL_TAG);

  // store 10 empty events
  for (size_t i{0}; i < 10; ++i) {
    datatools::things event;
    auto& mcd = event.add<mctools::simulated_data>("SD");
    mcd.set_time(3.14);
    // 'gg' hits
    mcd.add_step_hits("gg",10);
    for (size_t g{0}; g < 10; ++g) {
      auto& h = mcd.add_step_hit("gg");
      h.reset();
    }
    // 'calo' hits
    mcd.add_step_hits("calo",10);
    for (size_t c{0}; c < 10; ++c) {
      auto& h = mcd.add_step_hit("calo");
      h.reset();
    }

    output.store(event, "ER");
  }

  // See how to do same in XML file.

}

void read(const std::string& path) {
  std::clog << "Reading file `" << path << "`\n";

  // Open (brio) file
  brio::reader input{path};

  std::clog << "checking format... ";
  // Assert existence of "GI" and "ER" banks
  if (!input.has_store("GI")) {
    throw std::runtime_error("No GI store");
  }
  if (!input.has_store("ER")) {
    throw std::runtime_error("No ER store");
  }

  // Assert that:
  // - "GI" bank holds datatools::properties
  if (!input.has_store_with_serial_tag("GI", datatools::properties::SERIAL_TAG)) {
    throw std::runtime_error("GI store does not hold datatools::properties");
  }

  // - "ER" holds "datatools::things"
  if (!input.has_store_with_serial_tag("ER", datatools::things::SERIAL_TAG)) {
    throw std::runtime_error("ER store does not hold datatools::things");
  }

  std::clog << "ok\n";

  // Check we can read and dump entries
  // - GI back
  std::clog << "+- GI\n";
  {
    input.select_store("GI");
    size_t i{0};
    datatools::properties p;

    while (input.has_next()) {
      input.load_next(p);
      std::clog << "   +- Item " << i << "\n";
      p.tree_dump(std::clog, "", "      ");
      ++i;
    }
  }

  // - ER back
  //
  std::clog << "+- ER\n";
  {
    input.select_store("ER");
    size_t i{0};
    datatools::things p;

    while (input.has_next()) {
      input.load_next(p);
      std::clog << "   +- Item " << i << "\n";
      p.tree_dump(std::clog, "", "      ");
      // Dump all tags in things
      std::vector<std::string> tags;
      p.get_names(tags);
      for (auto const& t : tags) {
        auto& bank = p.get(t);
        std::clog << "          +- Bank: " << t << "(" << bank.get_serial_tag() << ")\n";
        auto dumper = dynamic_cast<const datatools::i_tree_dumpable*>(&bank);
        dumper->tree_dump(std::clog, "", "             ");
      }

      ++i;
    }
  }
}


int main(int argc, const char* argv[]) {
  if (argc != 3) {
    std::cerr << "must supply two arguments: mode [read|write], filename\n";
    return 1;
  }

  std::string ioMode {argv[1]};
  std::string fileName {argv[2]};

  if ( (ioMode != std::string{"read"}) && (ioMode != std::string{"write"}) ) {
    std::cerr << "first argument must be 'read' or 'write' ('" << ioMode << "') supplied\n";
    return 1;
  }

  if ( ioMode == "read" ) {
    read(fileName);
  }
  else if ( ioMode == "write" ) {
    write(fileName);
  }
  else {
    std::cerr << "error handling failed\n";
    return 1;
  }

  return 0;
}


