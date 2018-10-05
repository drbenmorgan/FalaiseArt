#include "snemo/services/VariantService.h"

#include "snemo/datamodel/MultiProperties.h"

#include <iostream>

namespace snemo {
VariantService::VariantService(fhicl::ParameterSet const&, art::ActivityRegistry& aRegistry)
    : resourceSvc_{art::ServiceHandle<ResourceService>()},
      variantSvc_{}
{
  // Register callback...
  aRegistry.sPreBeginRun.watch(this, &VariantService::preBeginRun);
  variantSvc_.set_logging(datatools::logger::PRIO_TRACE);
  std::cout << "VariantService Constructing...\n";
}

VariantService::~VariantService()
{
  std::cout << "VariantService Destructing... \n";
}

void VariantService::preBeginRun(art::Run const& /*newRun*/)
{
  std::cout << "VariantService::preBeginRun called\n";
  // Need to consume metadata from Run (this uses a hard coded InputTag
  // for testing
  //auto runMetaData = run.getValidHandle<MultiProperties>(art::InputTag{"SD"});

  // Prepare service
  variantSvc_.stop();
  // variantSvc_.configure(<datafromrun>
  variantSvc_.start();
}

} // namespace snemo

DEFINE_ART_SERVICE(snemo::VariantService)
