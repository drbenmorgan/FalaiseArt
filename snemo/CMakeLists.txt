# Add a simple bridge to Falaise types
# We can't read flsimulate files without these, but we
# don't want to build the whole of Falaise to do so.
add_subdirectory(datamodel)

# Processing i.e Producer modules
add_subdirectory(processing)

# We can use Art's supplied "simple_plugin" function
# This is just a wrapper around add_library, taking care
# of the naming convention and type of module
# The important bit for now is that the first element
# lists the short name of the module, the second the underlying
# type (source, module ={producer,...}), the remainder any
# additional libraries needed.
include_directories(${PROJECT_SOURCE_DIR})
simple_plugin(BrioInputSource "source" Bayeux::Bayeux snemo_datamodel_bridge)
simple_plugin(SDInputAnalyzer "module" Bayeux::Bayeux CLHEP)

# For an analyzer using TFileService, need to link to the services layer and
# and any additional ROOT components.
simple_plugin(MockCalibrationAnalyzer "module"
  Bayeux::Bayeux
  art_Framework_Services_Registry
  art_Framework_Services_Optional
  art_Framework_Services_Optional_TFileService_service
  ROOT::Hist)

add_subdirectory(test)
