# We can use Art's supplied "simple_plugin" function
# This is just a wrapper around add_library, taking care
# of the naming convention and type of module
# The important bit for now is that the first element
# lists the short name of the module, the second the underlying
# type (source, module ={producer,...})

# - Example input source, just produces empty events
simple_plugin(FLExampleInputSource "source")

# - Example producer, just single int for now
simple_plugin(FLExampleProducer "module")
simple_plugin(FLExampleConfigurableProducer "module")

# - Example filter, will use producer above

# - Example analyzer, will use producer/filter,
#   plus TFileServices
simple_plugin(FLExampleAnalyzer "module"
  # Aditional links to service/root libs
  art::art_Framework_Services_Optional_TFileService_service
  ROOT::Hist
  )
