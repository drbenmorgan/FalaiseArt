#include <iostream>

#include <bayeux/brio/reader.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/things.h>
#include <bayeux/mctools/simulated_data.h>
//#include "FLDataModels/timestamp.h"
//#include "FLDataModels/event_header.h"

int main(int argc, char *argv[]) {
  if (argc != 2 ) {
    std::cerr << "Wrong number of arguments\n";
    return 1;
  }

  std::string brioFile {argv[1]};
  std::cout << brioFile << "\n";

  brio::reader input;

  // File open will throw an exception if file doesn't exist
  // WILL NOT throw error if file isn't ROOT format though!
  try {
    input.open(argv[1]);
  }
  catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  // Can just tree dump at this point
  // Can see that without valid ROOT format file, there's
  // a valid TFile pointer, but nothing else.
  // What is typical behaviour for programs that open a file
  // of invalid format?
  input.tree_dump();

  // In a Falaise3 file, have two stores:
  // - "ER" of datatools::things objects (Event store)
  // - "GI" of datatools::properties objects (4 objects)
  // In a Falaise2 file, have two stores:
  // - "ER" of datatools::things objects (Event store)
  // - "GI" of datatools::properties objects (12 objects)
  // So no store name difference, but need to look at contents
  // of GI store
  //

  // NB: Only one store can be active at any one time, at least for
  // simple iteration (otherwise have to set label on every call)
  input.select_store("GI");
  datatools::properties giObject;
  unsigned short i {0};

  // This is the simplest loop over the contents of a store
  // NB: the currently active one!
  // Note that the loadXXX operations are relatively inefficient
  // as they do a large number of checks at each stage.
  while (input.has_next()) {
    input.load_next(giObject);
    std::cout << "+- Item " << i << "\n";
    giObject.tree_dump();
    ++i;
  }

  // Now can try ER store
  // Basically identical between Falaise 2/3
  // (Always single "SD" bank of mctools::simulated_data objects)
  input.select_store("ER");
  datatools::things event;
  i = 0;

  while(input.has_next()) {
    input.load_next(event);
    std::cout << "+- Event " << i << "\n";
    event.tree_dump();
    // Important to check that "handles" are unique (handle.unique()), and are
    // valid (handle.has_data)
    // Yep, can extract the basic object...
    // const mctools::simulated_data& data = event.get<mctools::simulated_data>("SD");
    // data.tree_dump();
    //

    // What about subobjects

    ++i;
  }

  std::cout << "End of program\n";

  return 0;
}
