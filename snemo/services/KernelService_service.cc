#include "snemo/services/KernelService.h"

#include "bayeux/bayeux.h"
#include "bayeux/datatools/kernel.h"

#include <iostream>

namespace snemo {

KernelService::KernelService(fhicl::ParameterSet const&, art::ActivityRegistry& aRegistry)
{
  std::cout << "KernelService Constructing...\n";
  // Wrap Bayeux initialization because we need resources from various Bayeux's modules:
  bayeux::initialize(0,0,0);

  aRegistry.sPreBeginRun.watch(this, &KernelService::preBeginRun);
}

KernelService::~KernelService()
{
  bayeux::terminate();
  std::cout << "KernelService Destructing... \n";
}

void KernelService::preBeginRun(art::Run const&)
{
  std::cout << "KernelService::preBeginRun called...\n";
}

} // namespace snemo

DEFINE_ART_SERVICE(snemo::KernelService)
