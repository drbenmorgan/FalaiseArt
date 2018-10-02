//! A program to deomstrate and test FHiCL's ability to
//! generate a hash for a ParameterSet.
//! Check whether ids are affected by ordering of elements and so on

#include <iostream>

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

using PSet_t = fhicl::ParameterSet;
using PSetID_t = fhicl::ParameterSetID;

PSet_t makeFromFile(const char* path)
{
  PSet_t pset;
  cet::filepath_lookup maker("FHICL_FILE_PATH");
  fhicl::make_ParameterSet(path, maker, pset);
  return pset;
}

int main(int argc, const char *argv[])
{
  // assume argv[1], argv[2] are paths that FHiCL can find
  if (argc != 3) {
    std::cerr << "error: requires two fhicl files to compare by id argument\n";
    return 1;
  }

  PSet_t refnPSet = makeFromFile(argv[1]);
  PSetID_t refnID = refnPSet.id();

  PSet_t testPSet = makeFromFile(argv[2]);
  PSetID_t testID = testPSet.id();

  bool idsAreSame {refnID == testID};

  std::cout << "=== Reference PSet:\n" << refnPSet.to_indented_string() << std::endl;
  std::cout << "=== Permuted PSet:\n" << testPSet.to_indented_string() << std::endl;

  std::cout << refnID
            << (idsAreSame ? " == " : " != ")
            << testID
            << std::endl;

  return idsAreSame ? 0 : 1;
}
