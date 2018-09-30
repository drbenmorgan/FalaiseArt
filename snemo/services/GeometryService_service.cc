#include "snemo/services/GeometryService.h"

#include "snemo/datamodel/MultiProperties.h"

#include "art/Framework/Principal/Run.h"

namespace snemo {

GeometryService::GeometryService(fhicl::ParameterSet const&, art::ActivityRegistry& aRegistry)
{
  // Register callback...
  aRegistry.sPreBeginRun.watch(this, &GeometryService::preBeginRun);
}

void GeometryService::preBeginRun(art::Run const& run)
{
  std::cout << "GeometryService::preBeginRun called\n";
  // Hard coded test of retrieving product
  auto metaData = run.getValidHandle<MultiProperties>(art::InputTag{"SD"});
  metaData->tree_dump();
}

} // namespace snemo

DEFINE_ART_SERVICE(snemo::GeometryService)
