# Add a simple bridge to Falaise types
# We can't read flsimulate files without these, but we
# don't want to build the whole of Falaise to do so.
add_subdirectory(datamodel)

# We can use Art's supplied "simple_plugin" function
# This is just a wrapper around add_library, taking care
# of the naming convention and type of module
# The important bit for now is that the first element
# lists the short name of the module, the second the underlying
# type (source, module ={producer,...}), the remainder any
# additional libraries needed.

# - Example input source, just produces empty events
simple_plugin(BrioInputSource "source" Bayeux::Bayeux snemo_datamodel_bridge)

add_subdirectory(test)
