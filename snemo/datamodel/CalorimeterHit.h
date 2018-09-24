#ifndef CALORIMETERHIT_HH
#define CALORIMETERHIT_HH

/// Rough Model of a calibrated calorimeter hit

#include "bayeux/geomtools/geom_id.h"

namespace snemo {

struct CalorimeterHit {
  double energy;
  double energyError;
  double time;
  double timeError;
  geomtools::geom_id geoID;
};

} // namespace snemo

#endif // CALORIMETERHIT_HH

