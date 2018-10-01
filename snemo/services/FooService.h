#ifndef SNEMO_SERVICES_FOOSERVICE_HH
#define SNEMO_SERVICES_FOOSERVICE_HH

//! Null service to confirm alphanumeric vs dependency construction
//  orrder of services
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "snemo/services/KernelService.h"

namespace snemo {
  class FooService {
   public:
    FooService(fhicl::ParameterSet const&, art::ActivityRegistry&);
    ~FooService();

    void preBeginRun(art::Run const&);
   private:
    // Depend on KernelService by depending on it
    art::ServiceHandle<KernelService> kernelSvc_;
  };
} // namespace snemo

// Macro to register with frameworl
DECLARE_ART_SERVICE(snemo::FooService, LEGACY)

#endif
