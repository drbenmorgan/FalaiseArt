#include "snemo/services/GeometryService.h"

namespace snemo {

GeometryService::GeometryService(fhicl::ParameterSet const&, art::ActivityRegistry& aRegistry)
{
  // Register callback...
  aRegistry.sPostBeginRun.watch(this, &GeometryService::postBeginRun);
}

void GeometryService::postBeginRun(art::Run const& /*run*/)
{
  std::cout << "GeometryService::postBeginRun called\n";
}

} // namespace snemo

DEFINE_ART_SERVICE(snemo::GeometryService)
