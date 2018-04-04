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


`third.fcl`
-----------

So far our empty events are not processed and no results are output to file.
As with the number of events, output to file can be controlled by a command
line argument or FHiCL parameter. In the former case, we can use the `-o` command
line argument to `art` which defaults output to ROOT format via the `RootOutput`
module. Though art output files are ROOT format, convention is to use the `.art`
extension to distinguish them from possible analysis level files (of which more later).

Using our previous `second.fcl` file, we can output the events to file via:

```console
$ art -c second.fcl -o second.art
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
source : {
  module_type : EmptyEvent
  maxEvents   : 10
  firstRun    :  1
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
```

In `physics:` we add the `op` list to define modules to be processed.
The `op` name is arbitrary and can be anything you like, as Art's syntax
can separate these lists from other settings. The list contains names of
modules to be processed, here, the `myOutput` module we defined later in
the `outputs` table. This seemingly odd way of defining things will help
later on when we define more complex processing paths and multiple outputs.
The script can be run like the others:

```console
$ art -c third.fcl
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
$ file third.fcl
third.art: ROOT file Version 61204 (Compression: 7)
```

Even with the output file name on the fcl file, we can override it from
the command line with the `-o` argument:

``` console
$ art -c third.fcl -o myfile.art
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
$ art -c fourth.fcl
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
$ art -s myinput.art -c fourth.fcl -o myoutput.art
...
```

As before, run `art --print-description RootInput` to find out what parameters
the module can take.



`fifth.fcl`
-----------

Even though we are not processing events, we can still demonstrate simple filtering
(i.e. cuts) of events based on their Run/SubRun/Event ids. Art provides a filter module
`EventIDFilter` with which we can do this. We'll use the output of `third.fcl` as the input,
and only select events with id 2,4 and 6,7,8. Looking at the output of `art --print-description EventIDFilter`:

```console
$ art --print-description EventIDFilter

====================================================================================================

    module_type : EventIDFilter (or "art/Framework/Modules/EventIDFilter")

        provider: art
        type    : filter
        source  : / [ external source ] /art/Framework/Modules/EventIDFilter_module.cc
        library : /Users/bmorgan/Software/Homebrew.git/lib/libart_Framework_Modules_EventIDFilter_module.dylib

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

To filter on this we need to add a `filters` table to the `physics` table and a trigger path for this
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

so we also need to add the filtering trigger path to `SelectEvents`. This leads to the fhicl file:

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
$ art -c fifth.fcl
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
$ art -c fifth_print.fcl
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


Going Further
=============

The above sections show the basic functionality of `art` and control of input, output
and event processing. To go further we need to define our experiment's data model so
that we can store this in events for processing.



