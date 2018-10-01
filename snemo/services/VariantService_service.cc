#include "snemo/services/VariantService.h"

#include "bayeux/bayeux.h"
#include "bayeux/datatools/kernel.h"

#include <iostream>

namespace snemo {

VariantService::VariantService(fhicl::ParameterSet const&, art::ActivityRegistry&)
    : kernelSvc_{art::ServiceHandle<KernelService>()}
{
  std::cout << "VariantService Constructing...\n";
}

VariantService::~VariantService()
{
  std::cout << "VariantService Destructing... \n";
}

} // namespace snemo

DEFINE_ART_SERVICE(snemo::VariantService)
