#ifndef SNEMO_SERVICES_GEOMETRYSERVICE_HH
#define SNEMO_SERVICES_GEOMETRYSERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "snemo/services/VariantService.h"

#include <memory>

// Ideally want to totally hide this, but Pimple pattern
// means forward decl of a new type, but don't have a
// new type in this case... Can do it with nesting
// in impl file, but messy.
namespace geomtools {
class manager;
}

namespace snemo {
  class GeometryService {
   public:
    GeometryService(fhicl::ParameterSet const&, art::ActivityRegistry&);
    ~GeometryService();

    // Function that will be called by framework, after Run is
    // created but before modules see it.
    void preBeginRun(art::Run const&);

    // No other accessors yet, but should implement *direct*
    // interfaces to things we need, not pointer to help impl...

   private:
    // Need VariantService, because the underlying geometry_manager
    // will rely on it...
    art::ServiceHandle<VariantService> variantSvc_;
    std::unique_ptr<geomtools::manager> pImpl_;
  };
} // namespace snemo

// Macro to register with frameworl
DECLARE_ART_SERVICE(snemo::GeometryService, LEGACY)

#endif // SNEMO_SERVICES_GEOMETRYSERVICE_HH

