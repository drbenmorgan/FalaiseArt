#include "snemo/services/GeometryService.h"

#include "snemo/datamodel/MultiProperties.h"

#include "art/Framework/Principal/Run.h"

#include "bayeux/geomtools/manager.h"

namespace snemo {

GeometryService::GeometryService(fhicl::ParameterSet const&, art::ActivityRegistry& aRegistry) :
    pImpl_ {std::make_unique<geomtools::manager>()}
{
  std::cout << "GeometryService Constructing\n";
  // Register callback...
  aRegistry.sPreBeginRun.watch(this, &GeometryService::preBeginRun);
  pImpl_->tree_dump(std::cout);
  // NB: Init of the manager here might be dependent on task
  // 1. For simulation, it's defined by the input parameters (see preBeginRun?)
  // 2. For processing, it's defined by what's in the Run level metadata
  // 3. That affects how preBeginRun is implemented...
  // At present we're only handling (2), so focus on that
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

  // Here we would reconfigure the geometry manager...
  // Extract the config from the rundata, construct a datatools::properties
  // instance
  // newConfig = extract_thing_from(metaData)
  // NB: *might* have uninited manager here, so would need to check then reset
  // NB: How then does overall setup work? From input fhicl file
  //     Of GeometryService, or from RunDat, if so how is latter populated
  //     e.g. in case of simulation?
  //     Also means need to write into RunData in certain cases (a producing
  //     service, maybe two different services?)
  // or immediately init.
  // pImpl_->reset();
  // pImpl->initialize(newConfig);
}

} // namespace snemo

DEFINE_ART_SERVICE(snemo::GeometryService)
