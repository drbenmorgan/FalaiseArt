#ifndef CALORIMETERHITCOLLECTION_HH
#define CALORIMETERHITCOLLECTION_HH

/// Rough model of a collection of Calorimeter Hits

#include "snemo/datamodel/CalorimeterHit.h"
#include <vector>

namespace snemo {
  using CalorimeterHitCollection = std::vector<CalorimeterHit>;
} // namespace snemo

#endif // CALORIMETERHITCOLLECTION_HH
