A Basic Art/FHiCL Walkthrough
=============================

Before implementing custom processing in Art, it is useful to learn
basics of use such as

- The command line interface and options
- Grammar and syntax of Art [FHiCL](https://cdcvs.fnal.gov/redmine/projects/fhicl) scripts

Though the sections below are largely self-contained, they should be consulted
together with the detailed information on FHiCL and art's FHiCL Domain-Specific Grammar
available at:

- [FHiCL Language Syntax and Semantics](https://cdcvs.fnal.gov/redmine/documents/327)
- [Art Framework FHiCL Grammar and Parameters](https://cdcvs.fnal.gov/redmine/projects/art/wiki/ART_framework_parameters)
- [Overall Art Guide](https://cdcvs.fnal.gov/redmine/projects/art/wiki)


Getting Started
===============

Check that you have `art` installed and set up as described in the
main [README.md](README.md). Assuming that you have a CVMFS or bundle
install, `art` can be set up as:

``` console
$ . <pathtoinstallbase>/setups
$ setup art v2_11_03 -q (e17|c2):debug
$
```

Use `e17:debug` on Linux, and `c2:debug` on macOS, for the argument to `-q`.
As FNAL's setup system for Art relies heavily on environment variables,
it's best to run the above in a "clean" shell with no custom settings for
things like `PATH`, `LD_LIBRARY_PATH`, and the `CC/CXX/FC` variables.
If there are any issues in the `setup` step, please raise an [Issue](https://github.com/drbenmorgan/FalaiseArt/issues)
or drop @drbenmorgan a mail. After setting up `art` check that you
can run the executable:

```console
$ art --version
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
art 2.11.03
```

Don't worry about the `FHICL_FILE_PATH` warning, it will not affect the initial
examples, and we'll cover its meaning and use later on.

If you want to remove the environment changes made by the `setup` command,
simply run

``` console
$ unsetup_all
unsetup -j art
...
unsetup -j xrootd
$
```

The Art Command Line Interface
==============================
Basic Usage
-----------
Like all console programs, `art` provides several command line arguments
to control high level behaviour. These can be listed via:

```console
$ art --help
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

  Usage: art <-c <config-file>> <other-options> [<source-file>]+

  Basic options:
    -h [ --help ]                         produce help message
...
$
```

The most important ones to look at are the first three sections on `Basic options:`,
`Source options:`, and `Output options:`. Later sections are for more advanced
processing and debugging work. Our basic workflow with Art will thus very similar
to Falaise, looking something like:

```console
$ art [--source <inputfile>] -c MyFhiclScript.fcl [--output <outputfile>]
```

The source and output files are optional, because as we'll see we can specify these
in the FHiCL script.

Just like Falaise, Art's functionality is implemented as a pipeline whose input,
processing, and output are controlled through a sequence of modules defined by
the FHiCL script. Art also implements the concept of "Services" that provide information that is not strictly
Event or Run level (for example, logging, profiling). These too can be configured
by the FHiCL script.


Getting Help on Modules and Services
------------------------------------
Art provides many useful builtin modules/services for non-experiment specific
tasks. We can find out what modules and services are known to Art, later on
including our own, by using the `--print-available <type>` argument. Here,
`<type>` refers to the type of task performed, which may be:

- `source`: A module that creates events and feeds them into the pipeline,
  for example from file.
- `module`: A module that handles processing of event data.
- `service`: A service living outside the pipeline and which may be accessed
  by `source` and `module` modules.

For art 2.11.3, running each will show:

``` console
$ art --print-available source
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

===========================================================================================================================================

   module_type                 Provider    Source location
-------------------------------------------------------------------------------------------------------------------------------------------
1. EmptyEvent                  art         / [ external source ] /art/Framework/Modules/EmptyEvent_source.cc
2. EventProcessorTestSource    art         / [ external source ] /art/test/Framework/EventProcessor/EventProcessorTestSource_source.cc
3. RootInput                   art         / [ external source ] /art/Framework/IO/Root/RootInput_source.cc

===========================================================================================================================================

$ art --print-available module
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

================================================================================================================================================

   module_type                 Type             Source location
------------------------------------------------------------------------------------------------------------------------------------------------
1. BlockingPrescaler           filter           / [ external source ] /art/Framework/Modules/BlockingPrescaler_module.cc
2. DataFlowDumper              output module    / [ external source ] /art/Framework/Modules/DataFlowDumper_module.cc
3. EventIDFilter               filter           / [ external source ] /art/Framework/Modules/EventIDFilter_module.cc
4. EventProcessorTestOutput    output module    / [ external source ] /art/test/Framework/EventProcessor/EventProcessorTestOutput_module.cc
5. FileDumperOutput            output module    / [ external source ] /art/Framework/Modules/FileDumperOutput_module.cc
6. Prescaler                   filter           / [ external source ] /art/Framework/Modules/Prescaler_module.cc
7. ProvenanceCheckerOutput     output module    / [ external source ] /art/Framework/Modules/ProvenanceCheckerOutput_module.cc
8. RandomNumberSaver           producer         / [ external source ] /art/Framework/Modules/RandomNumberSaver_module.cc
9. RootOutput                  output module    / [ external source ] /art/Framework/IO/Root/RootOutput_module.cc

================================================================================================================================================

$ art --print-available service
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

=======================================================================================================================

    Service                   Provider    Source location
-----------------------------------------------------------------------------------------------------------------------
 1. DatabaseConnection        art         / [ external source ] /DatabaseConnection_service.cc
 2. FileCatalogMetadata       art         / [ external source ] /FileCatalogMetadata_service.cc
 3. MemoryAdjuster            art         / [ external source ] /MemoryAdjuster_service.cc
 4. MemoryTracker             art         / [ external source ] /MemoryTracker_service.cc
 5. RandomNumberGenerator     art         / [ external source ] /RandomNumberGenerator_service.cc
 6. TFileService              art         / [ external source ] /TFileService_service.cc
 7. TimeTracker               art         / [ external source ] /TimeTracker_service.cc
 8. Tracer                    art         / [ external source ] /Tracer_service.cc
 9. TrivialFileDelivery       art         / [ external source ] /TrivialFileDelivery_service.cc
10. TrivialFileTransfer       art         / [ external source ] /TrivialFileTransfer_service.cc
11. floating_point_control    art         / [ external source ] /FloatingPointControl_service.cc
12. message                   art         [ See https://cdcvs.fnal.gov/redmine/projects/art/wiki/Messagefacility ]

=======================================================================================================================

$
```

We'll see in the coming sections how to use these in a FHiCL script. Art's
FHiCL system can also provide help on the FHiCL configuration required
by a given module or service, much like Falaise's OCD system. For example, one of the first modules we'll
use is the `EmptyEvent` source module, and we can see how to configure this
using:

```console
$ art --print-description EmptyEvent
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

====================================================================================================

    module_type: EmptyEvent (or "art/Framework/Modules/EmptyEvent")

        provider: art
        source  : / [ external source ] /art/Framework/Modules/EmptyEvent_source.cc
        library : /cvmfs/larsoft.opensciencegrid.org/products/art/v2_11_03/slf7.x86_64.e17.debug/lib/libart_Framework_Modules_EmptyEvent_source.so

    Allowed configuration
    ---------------------

        ## Any parameters prefaced with '#' are optional.

        source: {

           module_type: EmptyEvent

           maxEvents: -1  # default

           maxSubRuns: -1  # default

           reportFrequency: 1  # default

           errorOnFailureToPut: false  # default

           processingMode: "RunsSubRunsAndEvents"  # default

           numberEventsInRun: -1  # default

           numberEventsInSubRun: -1  # default

           eventCreationDelay: 0  # default

           resetEventOnSubRun: true  # default

         # firstRun: <unsigned int>

         # firstSubRun: <unsigned int>

         # firstEvent: <unsigned int>

           ## The 'timestampPlugin' parameter must be a FHiCL table
           ## of the form:
           ##
           ##   timestampPlugin: {
           ##     plugin_type: <plugin specification>
           ##     ...
           ##   }
           ##
           ## See the notes in art/Framework/Core/EmptyEventTimestampPlugin.h
           ## for more details.

         # timestampPlugin: << delegated >>
        }

====================================================================================================

$
```

This output is quite involved, so we defer explaining it until we actually write our
own scripts. Try using ``--print-description`` on some of the other modules and services.
Later on when we write our own modules, we'll see how to add this auto documentation
for them.


Example FHiCL Scripts
=====================

A series of basic FHiCL scripts are installed in the [fcl/examples](fcl/examples)
subdirectory. Art relies on absolute paths or relative paths plus the
`FHICL_FILE_PATH` environment variable to locate FHiCL scripts, so the following
assume that you are running `art` from the `fcl` subdirectory of your
clone of this project. In this case, `art` will set `FHICL_FILE_PATH` to the current working
directory, and we can pass the script paths relative to this.

The structure `art` expects FHiCL script to have is

```
# Inclusion of FHiCL "header" files

# Configuration of "services"
services: {
  # ...parameters...
}

# Configuration of Input Data Source
source: {
  # ...parameters...
}

# Configuration of Run/Event processing
physics: {
  # Configuration of modules that read/write data to the event, e,g. reconstruction
  producers: {
    # ...parameters...
  }

  # Configuration of modules that filter events, e.g. selection/cuts
  filters: {
    # ...parameters...
  }

  # Configuration of modules that read data, e.g. produce plots
  analyzers: {
    # ...parameters...
  }

  # Define processing paths/pipelines
  # 1. Can string together producers and filters
  #    Events flow through these in sequence
  myPipelinePath: [myFirstProducer, myFilter, mySecondProducer]

  # 2. Can list analyzers and outputs (see "output:" below)
  #    where events will be dumped after producer/filter paths finish
  myEndPath: [myAnalyzer, myOutputModule]
}

# Configuration of Output Data Sink
output: {
  # ... can have as many as we want, labels should be matched
  # with end paths in "physics:"
  myOutputModule: {
    # ...parameters...
  }
}
```


`zero.fcl`
----------

Art implements sensible defaults for processing, and so an empty (or rather, pure comment)
 file as shown in [fcl/examples/zero.fcl](fcl/examples/zero.fcl) is perfectly valid.
As shown above we pass this to `art` by passing it using the `-c` argument:

```console
$ art -c examples/zero.fcl
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
INFO: provided configuration file 'examples/zero.fcl' is empty:
using minimal defaults and command-line options.
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 20-Sep-2018 14:07:39 BST JobSetup
Messagelogger initialization complete.
%MSG
Begin processing the 1st record. run: 1 subRun: 0 event: 1 at 20-Sep-2018 14:07:40 BST

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 1 passed = 1 failed = 0

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 0.003854 Real = 0.004034

MemReport  ---------- Memory  Summary ---[base-10 MB]----
MemReport  VmPeak = 486.257 VmHWM = 156.262

Art has completed and will exit with status 0.
$
```

Your output will of course differ slightly depnding on time and exact system CPU.


`first.fcl`
-----------

An absolutely minimal script to create events and pass them through the pipeline.
All that's needed at this point is the `source` table to define where events will
come from. As we don't have input files or other event source yet, we use Art's
builtin `EmptyEvent` module to create, well, empty events. In the `source` table,
we just specify the module to be the source, and any further parameters which
for now we leave unset (`EmptyEvent` provides sensible defaults for us):

```
source : {
  module_type : EmptyEvent
}
```

We can now pass this script to `art` using the `-c` argument:

```console
$ art -c examples/first.fcl
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 18-Mar-2018 16:14:36 GMT JobSetup
Messagelogger initialization complete.
%MSG
Begin processing the 1st record. run: 1 subRun: 0 event: 1 at 18-Mar-2018 16:14:38 GMT

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 1 passed = 1 failed = 0

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 0.000843 Real = 0.000904

Art has completed and will exit with status 0.
$
```

Here, only one event has been generated. We can change this on the command line
using the `-n` argument. For example, to run 10 events:

``` console
$ art -c examples/first.fcl -n 10
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 18-Mar-2018 16:17:04 GMT JobSetup
Messagelogger initialization complete.
%MSG
Begin processing the 1st record. run: 1 subRun: 0 event: 1 at 18-Mar-2018 16:17:05 GMT
Begin processing the 2nd record. run: 1 subRun: 0 event: 2 at 18-Mar-2018 16:17:05 GMT
Begin processing the 3rd record. run: 1 subRun: 0 event: 3 at 18-Mar-2018 16:17:05 GMT
Begin processing the 4th record. run: 1 subRun: 0 event: 4 at 18-Mar-2018 16:17:05 GMT
Begin processing the 5th record. run: 1 subRun: 0 event: 5 at 18-Mar-2018 16:17:05 GMT
Begin processing the 6th record. run: 1 subRun: 0 event: 6 at 18-Mar-2018 16:17:05 GMT
Begin processing the 7th record. run: 1 subRun: 0 event: 7 at 18-Mar-2018 16:17:05 GMT
Begin processing the 8th record. run: 1 subRun: 0 event: 8 at 18-Mar-2018 16:17:05 GMT
Begin processing the 9th record. run: 1 subRun: 0 event: 9 at 18-Mar-2018 16:17:05 GMT
Begin processing the 10th record. run: 1 subRun: 0 event: 10 at 18-Mar-2018 16:17:05 GMT

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 10 passed = 10 failed = 0

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 0.001497 Real = 0.001605

Art has completed and will exit with status 0.
$
```


`second.fcl`
------------

This script extends `first.fcl` to show how the number of events to process can be
set in the FHiCL script. Most Art modules use FHiCL's [configuration validation](https://cdcvs.fnal.gov/redmine/projects/art/wiki/Configuration_validation_and_description)
system to bind FHiCL to C++ and provide documentation and validation of supplied
settings. Providing the module is known to Art, we can find out how to
configure a given module with the `--print-description <modulename>` argument to `art`.
For the `EmptyEvent` module, we can do:

``` console
$ art --print-description EmptyEvent
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

====================================================================================================

    module_type: EmptyEvent (or "art/Framework/Modules/EmptyEvent")

        provider: art
        source  : / [ external source ] /art/Framework/Modules/EmptyEvent_source.cc
        library : /cvmfs/larsoft.opensciencegrid.org/products/art/v2_11_03/slf7.x86_64.e17.debug/lib/libart_Framework_Modules_EmptyEvent_source.so

    Allowed configuration
    ---------------------

        ## Any parameters prefaced with '#' are optional.

        source: {

           module_type: EmptyEvent

           maxEvents: -1  # default

           maxSubRuns: -1  # default

           reportFrequency: 1  # default

           errorOnFailureToPut: false  # default

           processingMode: "RunsSubRunsAndEvents"  # default

           numberEventsInRun: -1  # default

           numberEventsInSubRun: -1  # default

           eventCreationDelay: 0  # default

           resetEventOnSubRun: true  # default

         # firstRun: <unsigned int>

         # firstSubRun: <unsigned int>

         # firstEvent: <unsigned int>
        }

====================================================================================================

Art has completed and will exit with status 1.
```

To set the number of events, we add the `maxEvents` parameter to the `source:` table:

```
source: {
  module_type : EmptyEvent
  maxEvents   : 10
}
```

We can run this in `art` and get 10 events generated:

```
$ art -c examples/second.fcl
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 18-Mar-2018 16:32:27 GMT JobSetup
Messagelogger initialization complete.
%MSG
Begin processing the 1st record. run: 1 subRun: 0 event: 1 at 18-Mar-2018 16:32:29 GMT
Begin processing the 2nd record. run: 1 subRun: 0 event: 2 at 18-Mar-2018 16:32:29 GMT
Begin processing the 3rd record. run: 1 subRun: 0 event: 3 at 18-Mar-2018 16:32:29 GMT
Begin processing the 4th record. run: 1 subRun: 0 event: 4 at 18-Mar-2018 16:32:29 GMT
Begin processing the 5th record. run: 1 subRun: 0 event: 5 at 18-Mar-2018 16:32:29 GMT
Begin processing the 6th record. run: 1 subRun: 0 event: 6 at 18-Mar-2018 16:32:29 GMT
Begin processing the 7th record. run: 1 subRun: 0 event: 7 at 18-Mar-2018 16:32:29 GMT
Begin processing the 8th record. run: 1 subRun: 0 event: 8 at 18-Mar-2018 16:32:29 GMT
Begin processing the 9th record. run: 1 subRun: 0 event: 9 at 18-Mar-2018 16:32:29 GMT
Begin processing the 10th record. run: 1 subRun: 0 event: 10 at 18-Mar-2018 16:32:29 GMT

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 10 passed = 10 failed = 0

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 0.001461 Real = 0.001562

Art has completed and will exit with status 0.
$
```

You can still pass the `-n` command line argument to override the number of events.

Part of the FHiCL validation system is helpful error messaging. For example,
if we spell the name of a parameter incorrectly, `art` will point to the location
and cause of the error:

``` console
$ art -c examples/second_error.fcl
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 18-Mar-2018 16:34:04 GMT JobSetup
Messagelogger initialization complete.
%MSG
%MSG-s ArtException:  Early 18-Mar-2018 16:34:05 GMT JobSetup
cet::exception caught in art
---- Configuration BEGIN


  Module label: source
  module_type : EmptyEvent

  Any parameters prefaced with '#' are optional.
  Unsupported parameters:

   + maxEvent                       [ ./second_error.fcl:14 ]

---- Configuration END
%MSG
Art has completed and will exit with status 9.
$
```

Try out some of the other parameters for `EmptyEvent` and see what happens


`third.fcl`
-----------

So far our empty events are not processed and no results are output to file.
As with the number of events, output to file can be controlled by a command
line argument or FHiCL parameter. In the former case, we can use the `-o` command
line argument to `art` which defaults output to ROOT format via the `RootOutput`
module. Though art output files are ROOT format, convention is to use the `.art`
extension to distinguish them from possible analysis level files.

Using our previous `second.fcl` file, we can output the events to file via:

```console
$ art -c examples/second.fcl -o second.art
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 18-Mar-2018 17:19:27 GMT JobSetup
Messagelogger initialization complete.
%MSG
%MSG-i FastCloning:  RootOutput:out@Construction 18-Mar-2018 17:19:29 GMT  ModuleConstruction
Initial fast cloning configuration (from default): true
%MSG
Begin processing the 1st record. run: 1 subRun: 0 event: 1 at 18-Mar-2018 17:19:29 GMT
18-Mar-2018 17:19:29 GMT  Opened output file with pattern "second.art"
%MSG-w FastCloning:  PostProcessEvent 18-Mar-2018 17:19:29 GMT  run: 1 subRun: 0 event: 1
Fast cloning deactivated for this input file due to empty event tree and/or event limits.
%MSG
Begin processing the 2nd record. run: 1 subRun: 0 event: 2 at 18-Mar-2018 17:19:29 GMT
Begin processing the 3rd record. run: 1 subRun: 0 event: 3 at 18-Mar-2018 17:19:29 GMT
Begin processing the 4th record. run: 1 subRun: 0 event: 4 at 18-Mar-2018 17:19:29 GMT
Begin processing the 5th record. run: 1 subRun: 0 event: 5 at 18-Mar-2018 17:19:29 GMT
Begin processing the 6th record. run: 1 subRun: 0 event: 6 at 18-Mar-2018 17:19:29 GMT
Begin processing the 7th record. run: 1 subRun: 0 event: 7 at 18-Mar-2018 17:19:29 GMT
Begin processing the 8th record. run: 1 subRun: 0 event: 8 at 18-Mar-2018 17:19:29 GMT
Begin processing the 9th record. run: 1 subRun: 0 event: 9 at 18-Mar-2018 17:19:29 GMT
Begin processing the 10th record. run: 1 subRun: 0 event: 10 at 18-Mar-2018 17:19:29 GMT
18-Mar-2018 17:19:30 GMT  Closed output file "second.art"

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 10 passed = 10 failed = 0

TrigReport ------ Modules in End-Path: end_path ------------
TrigReport  Trig Bit#        Run    Success      Error Name
TrigReport     0    0         10         10          0 out

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 1.027326 Real = 1.035404

Art has completed and will exit with status 0.
$
$ file second.art
second.art: ROOT file Version 61204 (Compression: 7)
$
```

We can also define the filename in the FHiCL file using the
`outputs` table (note the plural, unlike `source` we can have
multiple outputs eventually). However, because outputs are part
of processing, we must also add the `physics` table so we
can put the output in this:

```
source: {
  module_type : EmptyEvent
  maxEvents   : 10
  firstRun    :  1
}

physics: {
  op: [ myOutput ]
}

outputs: {
  myOutput : {
    module_type : RootOutput
    fileName    : "third.art"
  }
}
```

In `physics:` we add the `op` list to define modules to be processed.
The `op` name is arbitrary and can be anything you like, as Art's syntax
can separate these lists from other settings. The list contains names of
modules to be processed, here, the `myOutput` module we defined later in
the `outputs` table. This seemingly odd way of defining things will help
later on when we define more complex processing paths and multiple outputs.
The script can be run like the others:

```console
$ art -c examples/third.fcl
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 18-Mar-2018 17:47:56 GMT JobSetup
Messagelogger initialization complete.
%MSG
%MSG-i FastCloning:  RootOutput:myOutput@Construction 18-Mar-2018 17:47:57 GMT  ModuleConstruction
Initial fast cloning configuration (from default): true
%MSG
Begin processing the 1st record. run: 1 subRun: 0 event: 1 at 18-Mar-2018 17:47:57 GMT
18-Mar-2018 17:47:57 GMT  Opened output file with pattern "third.art"
%MSG-w FastCloning:  PostProcessEvent 18-Mar-2018 17:47:57 GMT  run: 1 subRun: 0 event: 1
Fast cloning deactivated for this input file due to empty event tree and/or event limits.
%MSG
Begin processing the 2nd record. run: 1 subRun: 0 event: 2 at 18-Mar-2018 17:47:57 GMT
Begin processing the 3rd record. run: 1 subRun: 0 event: 3 at 18-Mar-2018 17:47:57 GMT
Begin processing the 4th record. run: 1 subRun: 0 event: 4 at 18-Mar-2018 17:47:57 GMT
Begin processing the 5th record. run: 1 subRun: 0 event: 5 at 18-Mar-2018 17:47:57 GMT
Begin processing the 6th record. run: 1 subRun: 0 event: 6 at 18-Mar-2018 17:47:57 GMT
Begin processing the 7th record. run: 1 subRun: 0 event: 7 at 18-Mar-2018 17:47:57 GMT
Begin processing the 8th record. run: 1 subRun: 0 event: 8 at 18-Mar-2018 17:47:57 GMT
Begin processing the 9th record. run: 1 subRun: 0 event: 9 at 18-Mar-2018 17:47:57 GMT
Begin processing the 10th record. run: 1 subRun: 0 event: 10 at 18-Mar-2018 17:47:57 GMT
18-Mar-2018 17:47:58 GMT  Closed output file "third.art"

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 10 passed = 10 failed = 0

TrigReport ------ Modules in End-Path: end_path ------------
TrigReport  Trig Bit#        Run    Success      Error Name
TrigReport     0    0         10         10          0 myOutput

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 1.037451 Real = 1.045176

Art has completed and will exit with status 0.
$
$ file third.art
third.art: ROOT file Version 61204 (Compression: 7)
```

Even with the output file name in the fcl file, we can override it from
the command line with the `-o` argument:

``` console
$ art -c examples/third.fcl -o myfile.art
...
$
```


`fourth.fcl`
------------

Now we have an output Art file, we can use it as an event source. All we need
to do is change the `source` table to use the `RootInput` module which can read
from Art-format ROOT files:

```
source : {
  module_type : RootInput
  fileNames   : ["third.art"]
}

physics : {
  op: [ myOutput ]
}

outputs : {
  myOutput : {
    module_type : RootOutput
    fileName    : "fourth.art"
  }
}
```

Assuming that `third.art` exists in the current directory, then this can be
run as:

```console
$ art -c examples/fourth.fcl
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 18-Mar-2018 18:51:42 GMT JobSetup
Messagelogger initialization complete.
%MSG
18-Mar-2018 18:51:44 GMT  Initiating request to open input file "third.art"
18-Mar-2018 18:51:44 GMT  Opened input file "third.art"
%MSG-i FastCloning:  RootOutput:myOutput@Construction 18-Mar-2018 18:51:45 GMT  ModuleConstruction
Initial fast cloning configuration (from default): true
%MSG
Begin processing the 1st record. run: 1 subRun: 0 event: 1 at 18-Mar-2018 18:51:45 GMT
18-Mar-2018 18:51:45 GMT  Opened output file with pattern "fourth.art"
Begin processing the 2nd record. run: 1 subRun: 0 event: 2 at 18-Mar-2018 18:51:45 GMT
Begin processing the 3rd record. run: 1 subRun: 0 event: 3 at 18-Mar-2018 18:51:45 GMT
Begin processing the 4th record. run: 1 subRun: 0 event: 4 at 18-Mar-2018 18:51:45 GMT
Begin processing the 5th record. run: 1 subRun: 0 event: 5 at 18-Mar-2018 18:51:45 GMT
Begin processing the 6th record. run: 1 subRun: 0 event: 6 at 18-Mar-2018 18:51:45 GMT
Begin processing the 7th record. run: 1 subRun: 0 event: 7 at 18-Mar-2018 18:51:45 GMT
Begin processing the 8th record. run: 1 subRun: 0 event: 8 at 18-Mar-2018 18:51:45 GMT
Begin processing the 9th record. run: 1 subRun: 0 event: 9 at 18-Mar-2018 18:51:45 GMT
Begin processing the 10th record. run: 1 subRun: 0 event: 10 at 18-Mar-2018 18:51:45 GMT
18-Mar-2018 18:51:45 GMT  Closed output file "fourth.art"
18-Mar-2018 18:51:45 GMT  Closed input file "third.art"

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 10 passed = 10 failed = 0

TrigReport ------ Modules in End-Path: end_path ------------
TrigReport  Trig Bit#        Run    Success      Error Name
TrigReport     0    0         10         10          0 myOutput

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 0.134975 Real = 0.146336

Art has completed and will exit with status 0.
$
```

Filenames in the input and output modules can be specified as relative or full paths.
With a file based input module, we can also run the script using command line
specified input and output files, e.g

```console
$ art -s myinput.art -c examples/fourth.fcl -o myoutput.art
...
```

As before, run `art --print-description RootInput` to find out what parameters
the module can take.



`fifth.fcl`
-----------

Even though we are not processing events, we can still demonstrate simple filtering
(i.e. cuts) of events based on their Run/SubRun/Event ids. Art provides a filter module
`EventIDFilter` with which we can do this. We'll use the file output by `third.fcl` as the input,
and only select events with id 2,4 and 6,7,8. Looking at the output of `art --print-description EventIDFilter`:

```console
$ art --print-description EventIDFilter

====================================================================================================

    module_type : EventIDFilter (or "art/Framework/Modules/EventIDFilter")

        provider: art
        type    : filter
        source  : / [ external source ] /art/Framework/Modules/EventIDFilter_module.cc
        library : /cvmfs/larsoft.opensciencegrid.org/products/art/v2_11_03/slf7.x86_64.e17.debug/lib/libart_Framework_Modules_EmptyEvent_source.so

    Allowed configuration
    ---------------------

        ## Any parameters prefaced with '#' are optional.

        <module_label>: {

           module_type: EventIDFilter

           errorOnFailureToPut: true  # default

           ## The 'idsToMatch' parameter value is a sequence of patterns,
           ## each of which are composed three fields:
           ##
           ##   <run>:<subrun>:<event>
           ##
           ## Each of the run, subrun, and event fields can be represented
           ## by a number, or set of numbers.  The '*' wildcard can be used to
           ## represent any number, and the ',' and '-' characters can be used
           ## to sets or ranges of numbers.  For example:
           ##
           ##    "1:*:*"     // Accept Run 1, any SubRun, any Event
           ##    "1:2:*"     // Accept Run 1, SubRun 2, any Event
           ##    "1:2:3"     // Accept Run 1, SubRun 2, Event 3
           ##    "1:*:4"     // Accept Run 1, any SubRun, Event 4
           ##    "1:2-5:*"   // Accept Run 1, SubRuns 2 through 5 (inclusive), any Event
           ##    "*:9:10,11" // Accept any Run, SubRun 9, Events 10 and 11
           ##    "7:2-5,8:*" // Accept Run 7, SubRuns 2 through 5 (inclusive) and 8, any Event
           ##
           ## Specifying multiple patterns in the sequence corresponds to a
           ## logical OR of the patterns.  In other words, if the event in question
           ## matches any (not all) of the patterns, the event is accepted.

           idsToMatch: [
              <string>,
              ...
           ]
        }

====================================================================================================
```

To filter on this we need to add a `filters` table to the `physics` table and a path for this
filter. Because of the way art handles filtering, we also look at the `RootOutput` description and see:

```console
$ art --print-description RootOutput
...
    Allowed configuration
    ---------------------

        ## Any parameters prefaced with '#' are optional.

        <module_label>: {

           module_type: RootOutput

           ## The following parameter is a user-provided list
           ## of filter paths. The default list is empty.

           SelectEvents: [
           ]
...
```

so we also need to add the label of the processing/filtering path to `SelectEvents`. This leads to the fhicl file:

```
source : {
  module_type : RootInput
  fileNames   : ["third.art"]
}

physics: {
  filters: {
    myFilter: {
      module_type: EventIDFilter
      idsToMatch: [ "*:*:2,4,6-8" ]
    }
  }

  fp: [ myFilter ]
  op: [ myOutput ]
}

outputs: {
  myOutput : {
    module_type: RootOutput
    fileName: "fifth.art"
    SelectEvents: [ fp ]
  }
}
```

Running this through `art` gives:

```
$ art -c examples/fifth.fcl
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 04-Apr-2018 23:23:50 BST JobSetup
Messagelogger initialization complete.
%MSG
04-Apr-2018 23:23:51 BST  Initiating request to open input file "third.art"
04-Apr-2018 23:23:51 BST  Opened input file "third.art"
%MSG-i FastCloning:  RootOutput:myOutput@Construction 04-Apr-2018 23:23:52 BST  ModuleConstruction
Initial fast cloning configuration (from default): true
%MSG
%MSG-w FastCloning:  RootOutput:myOutput@Construction 04-Apr-2018 23:23:52 BST  ModuleConstruction
Fast cloning deactivated due to presence of
event selection configuration.
%MSG
Begin processing the 1st record. run: 1 subRun: 0 event: 1 at 04-Apr-2018 23:23:52 BST
04-Apr-2018 23:23:52 BST  Opened output file with pattern "fifth.art"
Begin processing the 2nd record. run: 1 subRun: 0 event: 2 at 04-Apr-2018 23:23:52 BST
Begin processing the 3rd record. run: 1 subRun: 0 event: 3 at 04-Apr-2018 23:23:52 BST
Begin processing the 4th record. run: 1 subRun: 0 event: 4 at 04-Apr-2018 23:23:52 BST
Begin processing the 5th record. run: 1 subRun: 0 event: 5 at 04-Apr-2018 23:23:52 BST
Begin processing the 6th record. run: 1 subRun: 0 event: 6 at 04-Apr-2018 23:23:52 BST
Begin processing the 7th record. run: 1 subRun: 0 event: 7 at 04-Apr-2018 23:23:52 BST
Begin processing the 8th record. run: 1 subRun: 0 event: 8 at 04-Apr-2018 23:23:52 BST
Begin processing the 9th record. run: 1 subRun: 0 event: 9 at 04-Apr-2018 23:23:52 BST
Begin processing the 10th record. run: 1 subRun: 0 event: 10 at 04-Apr-2018 23:23:52 BST
04-Apr-2018 23:23:52 BST  Closed output file "fifth.art"
04-Apr-2018 23:23:52 BST  Closed input file "third.art"

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 10 passed = 5 failed = 5

TrigReport ------ Modules in End-Path: end_path ------------
TrigReport  Trig Bit#        Run    Success      Error Name
TrigReport     0    0          5          5          0 myOutput

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 0.146595 Real = 0.166997

Art has completed and will exit with status 0.
$
```

We can see that all events were processed, but the `myOutput` module in the end path
only ran 5 times, as expected. We can confirm that these 5 events matched out event id
selection criteria via the `fifth_print.fcl` file:

```console
$ art -c examples/fifth_print.fcl
INFO: using default process_name of "DUMMY".
%MSG-i MF_INIT_OK:  Early 04-Apr-2018 23:26:02 BST JobSetup
Messagelogger initialization complete.
%MSG
04-Apr-2018 23:26:04 BST  Initiating request to open input file "fifth.art"
04-Apr-2018 23:26:04 BST  Opened input file "fifth.art"
Begin processing the 1st record. run: 1 subRun: 0 event: 2 at 04-Apr-2018 23:26:07 BST
Begin processing the 2nd record. run: 1 subRun: 0 event: 4 at 04-Apr-2018 23:26:07 BST
Begin processing the 3rd record. run: 1 subRun: 0 event: 6 at 04-Apr-2018 23:26:07 BST
Begin processing the 4th record. run: 1 subRun: 0 event: 7 at 04-Apr-2018 23:26:07 BST
Begin processing the 5th record. run: 1 subRun: 0 event: 8 at 04-Apr-2018 23:26:07 BST
04-Apr-2018 23:26:07 BST  Closed input file "fifth.art"

TrigReport ---------- Event  Summary ------------
TrigReport Events total = 5 passed = 5 failed = 0

TimeReport ---------- Time  Summary ---[sec]----
TimeReport CPU = 0.033540 Real = 0.041040

Art has completed and will exit with status 0.
$
```

We see the event ids are as expected. For further info on filtering and paths, see
the art Wiki on [paths](https://cdcvs.fnal.gov/redmine/projects/art/wiki/Paths)
and [filtering](https://cdcvs.fnal.gov/redmine/projects/art/wiki/Filtering_events).


`sixth.fcl`
-----------

FHiCL scripts can, much like C/C++, include others to provide a standard set of parameters.
The exact syntax of this feature is discussed in Section 9 of the [FHiCL Quickstart Guide](https://cdcvs.fnal.gov/redmine/documents/327).
In the simplest case, we can reproduce the behaviour of the `third.fcl` script via
including it in a new script:

```
#include "examples/third.fcl"
```

Running this in `art` yields identical results to before:

```console
$ art -c examples/sixth.fcl
... output as for third.fcl ...
$
```

The `#include` mechanism is extremely useful to separate concerns and provide
packaged, experiment specific configurations for things like processing modules.


`seventh.fcl`
-------------

Unless specified, FHiCL allows parameter values to be overridden in scripts, with
a "latest wins" policy. In combination with `#include`, we can

- Include a packaged configuration script
- Override 1-N parameters to adjust behaviour.

For example, we can reuse our `third.fcl` configuration, and override the number of events
and output file:

```
#include "examples/third.fcl"

source: {
  maxEvents: 100
}

outputs.myOutput.fileName: "seventh.art"
```

Running this as

```console
$ art -c examples/seventh.fcl
```

should produce similar output to the third example, and yield a `seventh.art`
output file.

This can easily be extended to more complex tasks, e.g. profiling
changes to a few reconstruction parameters.

Note that FHiCL does require that all overriden parameters be "fully
qualified". See Section 5.4 "Table Values" of the [FHiCL Quickstart Guide](https://cdcvs.fnal.gov/redmine/documents/327)


`eighth.fcl`
------------

Straight inclusion of a "bare" script is allowed by the FHiCL grammar, but it is
recommended to package scripts for inclusion as "Prologs". Section 8 on Prologs
in the [FHiCL Quickstart Guide](https://cdcvs.fnal.gov/redmine/documents/327)
covers these in more detail, the main message being that they help to keep
processing clean and prevent name clashes.

In [fcl/examples/empty_event.fcl](fcl/examples/empty_event.fcl), we've put the
configuration for the `EmptyEvent` module as a nested table inside a prolog.
In [fcl/examples/eighth.fcl](fcl/examples/eighth.fcl), we include this script
and use FHiCL's "reference" mechanism to use it as our source:

```
source : {
  @table::falaise.sources.empty_event
}
```

See Section 7 on References in the [FHiCL Quickstart Guide](https://cdcvs.fnal.gov/redmine/documents/327)
for full details on this syntax.


`ninth.fcl`
-----------

FHiCL parameters are mutable, i.e. can be overriden as shown earlier, by default.
We may have use cases where we need to provide a parameter, but it should not
be allowed to be overriden. Parameters can be marked as (effectively) "read only"
with a special `@protect_error` syntax (see Section 10 of the [FHiCL Quickstart Guide](https://cdcvs.fnal.gov/redmine/documents/327)).
In [fcl/examples/protected_empty_event.fcl](fcl/examples/empty_event.fcl), we
use this to lock down the `maxEvents` parameter:

```
BEGIN_PROLOG
falaise : {
  sources : {
    empty_event: {
      module_type: EmptyEvent
      # Try to substitute this later will yield an error
      maxEvents @protect_error: 10
      firstRun: 1
    }
  }
}
END_PROLOG
```

In [fcl/examples/ninth.fcl](fcl/examples/ninth.fcl), we try to override
`maxEvents`:

```
#include "examples/protected_empty_event.fcl"

source : {
  @table::falaise.sources.empty_event
}

physics : {
  op: [ myOutput ]
}

outputs : {
  myOutput : {
    module_type : RootOutput
    fileName    : "third.art"
  }
}

source.maxEvents: 75
```

but running this thorugh art will yield an error:

```
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."
Failed to parse the configuration file 'examples/ninth.fcl' with exception
---- Parse error BEGIN
  Error in assignment:
  ---- Protection violation BEGIN
    Part "maxEvents" of specification to be overwritten
    "source.maxEvents" is protected on line 11 of file "./examples/ninth.fcl"
  ---- Protection violation END
   at line 29, character 1, of file "./examples/ninth.fcl"

  source.maxEvents: 75
  ^
---- Parse error END

Art has completed and will exit with status 90.
$
```

Art is helpful here in telling us why the error occurred, together with
the file and line numbers related to it.


Going Further
=============

The above sections show the basic functionality of `art` and control of input, output
and event processing using FHiCL scripting. To go further we need to define our experiment's data model so
that we can store this in events for processing. These steps are covered in ...TODO...



