#ifndef SNEMO_SERVICES_GEOMETRYSERVICE_HH
#define SNEMO_SERVICES_GEOMETRYSERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"

namespace snemo {
  class GeometryService {
   public:
    GeometryService(fhicl::ParameterSet const&, art::ActivityRegistry&);

    // No other accessors yet

    // Function that will be called by framework, after Run is
    // created but before modules see it.
    void preBeginRun(art::Run const&);
  };
} // namespace snemo

// Macro to register with frameworl
DECLARE_ART_SERVICE(snemo::GeometryService, LEGACY)

#endif // SNEMO_SERVICES_GEOMETRYSERVICE_HH

