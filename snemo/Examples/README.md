Falaise/Art Example Modules
===========================

Minimal implementations of main Art processing module types to
demonstrate functionality.

Note that there are no header files. This is deliberate as art
only allows modules to communicate with each other via data,
so they have no need of a public interface.

Note also the naming of the files in the form `NAME_<type>.cc`. Here,
`<type>` is the category of plugin that the file implements. This
may be:

- `module` for producers, analyzers and filters
- `source` for input sources
- `service` for services

This naming is required by Art's `simple_plugin` CMake function.

See also the main [art Wiki](https://cdcvs.fnal.gov/redmine/projects/art/wiki)
which the following sections will refer to.


`FLExampleInputSource_source.cc`
--------------------------------

Example of an input source of events, e.g. from a file, stream
or event generator. In Falaise/Bayeux, the equivalent is `dpp::input_module`.

Art can read its own files, but if we want to
read from a custom file format we must provide an implementation for this.
This file shows a minimal implementation of the interface described here:

- https://cdcvs.fnal.gov/redmine/projects/art/wiki/Writing_your_own_input_sources

The key member function is `readNext(...)`. TODO: Document behaviour of
input/output `RunPrincipal` and `SubRunPrincipal` objects.

NB: Input sources can also be event generators, but we can also use a combination
of the `EmptyEvent` input source followed by a `EDProducer` module (See
`art --print-description EmptyEvent` for details of the former).


`FLExampleProducer_module.cc`
-----------------------------

Example of a concrete class of `art::EDProducer`, a module that can read from
and write to an `art::Event`. In Falaise/Bayeux, the equivalent is a concrete
class of `dpp::base_module`.

It shows that minimal interface such a module has,
with configuration at construction time via a bare `fhicl::ParameterSet` object,
and it "produces" (i.e. writes to the event) a single integer.

The key member functions are the constructor, which configures the module
from FHiCL input and declares what the module produces/consumes, and
`produce`, which reads/writes the event.

See also the art Wiki entries on [Module design guidelines](https://cdcvs.fnal.gov/redmine/projects/art/wiki/Art_Module_Design_Guide) and [Data product declarations](https://cdcvs.fnal.gov/redmine/projects/art/wiki/Data_product_declarations).


`FLExampleConfigurableProducer_module.cc`
-----------------------------------------

Identical to `FLExampleProducer_module.cc` except that configuration uses
FHiCL's [Configuration Validation](https://cdcvs.fnal.gov/redmine/projects/art/wiki/Configuration_validation_and_description) system instead of a bare `ParameterSet`.
The primary benefits of this are

1. art/FHiCL will automatically generate/print documentation on the configuration,
   available via `art --print-description <modulename>`.
2. art/FHiCL will validate that the supplied FHiCL text against the expected parameters
   declared by the module, raising errors with indications of the problem when they occur.
3. Configuration logic is located solely in the configuration struct/FHiCL type, so
   the module can focus on data, not configuration, processing.

See also the art Wiki entries on [recommended use and enabling config validation in modules](https://cdcvs.fnal.gov/redmine/projects/art/wiki/Configuration_validation_and_description) and
the dedicated FHiCL wiki on [configuration validation](https://cdcvs.fnal.gov/redmine/projects/fhicl-cpp/wiki/Configuration_validation_and_fhiclcpp_types)

In Falaise/Bayeux, the closest equivalent is Bayeux's OCD functionality, but this only
covers documentation, not validation. It is also completely separate to the processing/validation
of the input config, which the user must handle by hand, and keep in sync with the OCD info.

`FLExampleAnalyzer_module.cc`
-----------------------------

Example of a concrete class of `art::EDAnalyser`, a module that can
only read data from `art::Event` instances. In Falaise/Bayeux, the closest equivalent is a concrete
class of `dpp::base_module`, but this can always read and write.

It demonstrates a
typical use case for such a module: reading data from each event and
filling a histogram. To enable this it uses art's `TFileService`, a
builtin ["service"](https://cdcvs.fnal.gov/redmine/projects/art/wiki/Guide_to_writing_and_using_services)
for managing user level ROOT objects.

It shows the simplest possible use of `TFileService`. The ROOT
object(s) are created at the start of the `Job` (the highest level of
art processing) in the `beginJob` member function. Art will create
all ROOT objects, in this case a single `TDirectory` containing a
`TH1F`, into a `TDirectory` named by the module's label (its name
in the FHiCL configuration).

See also the art Wiki [documentation on `TFileService`](https://cdcvs.fnal.gov/redmine/projects/art/wiki/TFileService).
Note in particular the "file switching" functionality which gives
more control over the level (Job/Run/SubRun/Event) at which files will be
closed/opened.


Building Art Modules
====================
Art provides a basic CMake function `simple_plugin` to build shared libraries
for modules. This is a wrapper around CMake's `add_library` and takes care of
naming the library to conform with Art's naming convention and linking to the
minimal set of Art libraries required by a given module type.

As discussed earlier, the source files are named for the type of plugin they
implement, e.g. "MyPlugin_module.cc" for producer/analyser/filter modules.
We then only need call `simple_plugin` with the main name and type (yes, it's
a bit odd):

```cmake
simple_plugin(MyPlugin "module")
```

If the module links to additional libraries, they should be listed after these
two arguments, as target names or cmake variables as appropriate e.g.

```cmake
simple_plugin(MyPlugin "module" Another::Lib ${AdditionalLib})
```

