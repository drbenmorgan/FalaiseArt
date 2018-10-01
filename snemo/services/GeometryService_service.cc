#include "snemo/services/GeometryService.h"

#include "snemo/datamodel/MultiProperties.h"

#include "art/Framework/Principal/Run.h"

namespace snemo {

GeometryService::GeometryService(fhicl::ParameterSet const&, art::ActivityRegistry& aRegistry)
{
  std::cout << "GeometryService Constructing\n";
  // Register callback...
  aRegistry.sPreBeginRun.watch(this, &GeometryService::preBeginRun);
}

GeometryService::~GeometryService()
{
  std::cout << "GeometryService Destructing\n";
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
