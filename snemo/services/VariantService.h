#ifndef SNEMO_SERVICES_VARIANTSERVICE_HH
#define SNEMO_SERVICES_VARIANTSERVICE_HH

//! Service to manage Bayeux's "Variant" system

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "snemo/services/KernelService.h"

namespace snemo {
  class VariantService {
   public:
    VariantService(fhicl::ParameterSet const&, art::ActivityRegistry&);
    ~VariantService();

   private:
    // Depend on KernelService by depending on it
    art::ServiceHandle<KernelService> kernelSvc_;
  };
} // namespace snemo

// Macro to register with frameworl
DECLARE_ART_SERVICE(snemo::VariantService, LEGACY)

#endif
