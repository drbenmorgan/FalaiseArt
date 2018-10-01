#include "snemo/services/FooService.h"

#include "bayeux/bayeux.h"
#include "bayeux/datatools/kernel.h"

#include <iostream>

namespace snemo {

FooService::FooService(fhicl::ParameterSet const&, art::ActivityRegistry& aRegistry)
    : kernelSvc_{art::ServiceHandle<KernelService>()}
{
  aRegistry.sPreBeginRun.watch(this, &FooService::preBeginRun);
  std::cout << "FooService Constructing...\n";
}

FooService::~FooService()
{
  std::cout << "FooService Destructing... \n";
}

void FooService::preBeginRun(art::Run const&)
{
  std::cout << "FooService::preBeginRun called...\n";
}
} // namespace snemo

DEFINE_ART_SERVICE(snemo::FooService)
