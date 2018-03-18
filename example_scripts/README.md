Example Scripts
===============

Simple [FHiCL](https://cdcvs.fnal.gov/redmine/projects/fhicl) scripts for art, demonstrating
basic structure without, and then with, user modules and data.

Though the sections below are largely self-contained, additional information
on FHiCL and art's FHiCL DSL are available at:

- [FHiCL Syntax and Semantics](https://cdcvs.fnal.gov/redmine/documents/327)
- [Art Framework Parameters](https://cdcvs.fnal.gov/redmine/projects/art/wiki/ART_framework_parameters)
- [Overall Art Guide](https://cdcvs.fnal.gov/redmine/projects/art/wiki)


Getting Started
===============

Check that you can run `art` and get help on its plugins:

```console
$ art --help
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

  Usage: art <-c <config-file>> <other-options> [<source-file>]+

  Basic options:
    -h [ --help ]                         produce help message
...
$
$ art --print-available-modules
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

==================================================================================================================================

   module_type                Type             Source location
----------------------------------------------------------------------------------------------------------------------------------
1. BlockingPrescaler          filter           / [ external source ] /art/Framework/Modules/BlockingPrescaler_module.cc
2. DataFlowDumper             output module    / [ external source ] /art/Framework/Modules/DataFlowDumper_module.cc
3. EventIDFilter              filter           / [ external source ] /art/Framework/Modules/EventIDFilter_module.cc
4. FileDumperOutput           output module    / [ external source ] /art/Framework/Modules/FileDumperOutput_module.cc
5. Prescaler                  filter           / [ external source ] /art/Framework/Modules/Prescaler_module.cc
6. ProvenanceCheckerOutput    output module    / [ external source ] /art/Framework/Modules/ProvenanceCheckerOutput_module.cc
7. RandomNumberSaver          producer         / [ external source ] /art/Framework/Modules/RandomNumberSaver_module.cc
8. RootOutput                 output module    / [ external source ] /art/Framework/IO/Root/RootOutput_module.cc

==================================================================================================================================

Art has completed and will exit with status 1.
$
$ art --print-description EmptyEvent
Expected environment variable FHICL_FILE_PATH is missing or empty: using "."

====================================================================================================

    module_type: EmptyEvent (or "art/Framework/Modules/EmptyEvent")

        provider: art
        source  : / [ external source ] /art/Framework/Modules/EmptyEvent_source.cc
        library : /Users/bmorgan/Software/Homebrew.git/lib/libart_Framework_Modules_EmptyEvent_source.dylib

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
$
```

Any errors in the commands are likely due to missing settings for the dynamic loader path
(`LD_LIBRARY_PATH` on Linux, `DYLD_LIBRARY_PATH` on macOS). Don't worry about the
"_Expected environment variable FHICL_FILE_PATH is missing or empty: using "."_" warnings.
These are not relevant until later in the examples.


Example FHiCL Scripts
=====================

`first.fcl`
-----------

An absolutely minimal script to create events and pass them through the pipeline.
All that's needed at this point is the `source` table to define where events will
come from. As we don't have input files or other event source yet, we use Art's
builtin `EmptyEvent` module to create, well, empty events. In the `source` table,
we just specify the module to be the source, and any further parameters which
for now we leave unset (`EmptyEvent` provides sensible defaults us):

```
source : {
  module_type : EmptyEvent
}
```

We can now pass this script to `art` using the `-c` argument (assuming
we are in the same directory as the script, otherwise supply the full relative
or absolute path):

```console
$ art -c first.fcl
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
$ art -c first.fcl -n 10
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
        library : /Users/bmorgan/Software/Homebrew.git/lib/libart_Framework_Modules_EmptyEvent_source.dylib

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

To set the number of events, we use the `maxEvents` parameter:

```
source : {
  module_type : EmptyEvent
  maxEvents   : 10
}
```

We can run this in `art` and get 10 events generated:

```
$ art -c second.fcl
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

Part of the FHiCL validation system is helpful error messaging. For example,
if we spell the name of a parameter incorrectly, `art` will point to the location
and cause of the error:

``` console
$ art -c second_error.fcl
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
