// Cannot read brio file from Falaise/Bayeux
// using G4 and C++11. Not clear is this is
// ROOT/Boost/CXXSTD related, so try an xml
// file to reduce to level of class registration
//
//
#include <datatools/io_factory.h>

#include <datatools/properties.h>

#include <datatools/things.h>

int main() {
  datatools::data_reader reader {"test.xml", datatools::using_multiple_archives};
  reader.dump();
  datatools::properties a;
  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  reader.load(a);
  a.tree_dump();
  reader.dump();

  // Now have the things...
  datatools::things event;
  reader.load(event);
  std::vector<std::string> banks;
  event.get_names(banks);
  for(auto const& b: banks) {
    const datatools::i_serializable& elem = event.get(b);
    const datatools::i_tree_dumpable *dumpable
        = dynamic_cast<const datatools::i_tree_dumpable*>(&elem);
    dumpable->tree_dump(std::cout);
  }

  reader.dump();


  return 0;
}
