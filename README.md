FalaiseArt
==========

Experimental implementation of [Falaise](https://github.com/supernemo-dbd/falaise) using
the [art](https://art.fnal.gov) framework for data management and processing.


Installing Art
==============

1. From OpenScienceGrid CVMFS. This assumes an install of CVMFS
   with configuration for the `larsoft.opensciencegrid.org` repo.
   It is strongly recommended to use the bash shell here.
   T/Cshell is in principle supported, but issues have been seen on CentOS7 systems

   ```
   $ cvmfs_config probe
   $ source /cvmfs/larsoft.opensciencegrid.org/products/setups
   $ setup art v2_11_03 -q e17:debug
   $ art --help
   ```

   This gets you a basic runtime environment for Art, but only for the
   current version(s) supported for the LArSoft project.

2. Download relocatable binary bundles. This uses the bundles documented
   on the [Art wiki](https://cdcvs.fnal.gov/redmine/projects/cet-is-public/wiki/Get_binary_distributions).

   Bundles for art are available under [bundles/art](https://scisoft.fnal.gov/scisoft/bundles/art/).
   Choose the version you want (this project is currently based on 2.11.3)
   and go into its subdirectory. Click on the `art-v<VERSION>.html` link
   for the documentation on how to install that version. TL;DR:

   ```
   $ curl -O http://scisoft.fnal.gov/scisoft/bundles/tools/pullProducts
   $ chmod u+x ./pullProducts
   $ ./pullProducts -f -r <installprefix> <slf6|slf7|u16|d17> <e17|c2> debug
   ```

   Use slf6 (CentOS6), slf7 (CentOS7), u16 (Ubuntu 16.04 LTS) or d17 (
   macOS High Sierra) as appropriate for your system. Linux systems
   should prefer the `e17` (GCC 7, C++17), and macOS the `c2` (Clang C++17),
   qualifier. The `-f` flag forces re-download of tarballs if they exist
   (useful if the connection times out) and `-r` removes downloaded tarballs
   after they are unpacked. The `<installprefix>` must exist first, and should
   be empty.

   Note that the scisoft server is on a slow connection, so you will
   have to be patient waiting for the downloads. The bundles are also
   rather large as they include an almost complete record of the build.
   Currently, `v2_11_03` requires around 6GB of space.

   The installed products can be used in identical fashion to the CVMFS version:

   ``` console
   $ source <installprefix>/setups
   $ setup art v2_11_03 -q e17:debug
   $ art --help
   ```

3. _Experimental_ Docker or Singularity Images. A basic docker image bundling an install
   of Art and development tools is available from [Docker Hub](https://hub.docker.com/r/benmorgan/falaise-art/)

   The image may be run through Docker or Singularity, e.g.

   ```console
   $ docker run --rm -it benmorgan/falaise-art
   ...
   falaise-art> art-brew --help
   ```

   or

   ```console
   $ singularity shell docker://benmorgan/falaise-art:latest
   ...
   $ art-brew --help
   ```

   Note that the `art` executable here is named `art-brew` to provide the
   neccessary environment wrapping to run outside of FNAL's UPS system.

Getting Started
===============
Basic Art Usage
---------------

The `art` executable is used in a very similar fashion to `flsimulate` and
`flreconstruct`. It implements a pipeline pattern for processing event-based
data, with input/output from/to sources/sinks (which may be files or code).
Input, Output, and Modules in the pipeline are configured via a configuration
script in the JSON/YAML-like [FHiCL language](https://cdcvs.fnal.gov/redmine/projects/fhicl).

Even without any coding, it's possible to explore the basic functionality and
behaviour of Art and FHiCL with some simple examples. These are documented by
a basic walkthrough in the [README_examples.md](README_examples.md) file. Please
see [that document](README_examples.md) for further information.


Building the FalaiseArt Plugins
--------------------------------

To build and use the FalaiseArt plugins, a couple of additional steps are needed,
depending on whether you are running using CVMFS/UPS Bundles or Docker/Singularity
Containers.

For UPS, the initial setup of art is similar, but two additional packages are required:

``` console
$ source <productbasedir>/setups
$ setup art v2_11_03 -q e17:debug
$ setup cetbuildtools v7_04_00
$ setup ninja v1_8_2
```

Art is setup as before, the additional `setup`s adding the FNAL CMake modules for compiling
UPS based code, and the `ninja` build tool. There is nothing special going on here apart
from the oddities of the UPS based install tree and environment setup. Ninja is used to
speed up the build over the default `make` system, especially when using a CVMFS
install of Art.

With Docker images, no setup is required as the Container will drop you into the
needed environment, ready to go. When running the Docker image with Singularity
you will need to do:

``` console
$ singularity shell docker://benmorgan/falaise-art:latest
...
$ brew sh --cc=gcc-7 --env=std
...
```

This will drop you into the same environment as the Docker Container provides.

Just like Falaise, you should create a build directory for development and then run `cmake`,
pointing it to the source directory, e.g.

``` console
$ ls
FalaiseArt.git
$ mkdir mybuild
$ cd mybuild
$ cmake -GNinja ../FalaiseArt.git
...
```

CMake will run as standard, and should warn you if any required setup is missing. Simply type `ninja`
to build everything:

``` console
$ ninja
...
```

To keep things simple in the prototype stage, installing FalaiseArt is not yet implemented.
As in production Falaise, all executables and tasks can be run as-is from the build directory.
This provides sufficient functionality for testing and prototyping.


Using FalaiseArt Plugins
-------------------------

Due to Art's current reliance on environment variables for configuration, a simple wrapper script
`flreconstruct` is provided in the build directory of FalaiseArt to assist in running it with appropriate
settings. It simply forwards on all command line arguments to `art` itself, and so supports all of `art`'s
arguments, e.g.

```console
$ ./flreconstruct --help
```

The wrapper ensures all FalaiseArt plugins are available, and these can be listed using the various `--print-description[-TYPE]`. Art prints plugins in alphanumeric order, so you will see Art builtin modules mixed in with FalaiseArt ones.
Currently availble modules are:

- `BrioInputSource`: source module to read BRIO files output by production simulation, and populate Run and Event level products in Art.
- `MockCalorimeterCalibrator`: producer module that transforms raw steps in the calorimeter to calibrated versions
- `MockGeigerCalibrator`: (_not implemented yet_) producer module that transforms raw steps in the Geiger cels to calibrated versions
- `SDInputAnalyzer`: analyzer module to dump products produced by the "SD" module (effectively the `BrioInputSource`). It is a simple demonstration of how to "consume" data products from the Event.
- `MockCalibrationAnalyzer`: analyzer module that consumes products produced by `MockCalorimeterCalibrator` and uses Art's [`TFileService`](https://cdcvs.fnal.gov/redmine/projects/art/wiki/TFileService) to create and populate a ROOT `TH1F` of the total deposited energy.

Only `MockCalorimeterCalibrator` provides user configurable parameters at present, which can be viewed via `--print-description`:

```
$ ./flreconstruct --print-description MockCalorimeterCalibrator
...
```

The basic sequence of operations that can thus be implemented with the prototype is:

1) Obtain input BRIO file(s) from _production_ version of `flsimulate` in Falaise
2) Write an Art script that:
   - Uses `BrioInputSource` to read the `flsimulate` BRIO file
   - Adds `Mock{Calorimeter,Geiger}Calibrator` modules in a producer path
   - Adds `MockCalibrationAnalyzer` in an end path
   - Configure `TFileService` to write histogram created by the analyzer to file

The complete (i.e. _not_ using `#include` to package parameters) FHiCL script to do this is supplied in
[fcl/snemo_examples/CalibrationAnalysis.fcl](fcl/snemo_examples/CalibrationAnalysis.fcl)

```
services: {
  RandomNumberGenerator: {}
  TFileService: {
    fileName: "MyCalibrationAnalysis.root"
  }
}

source: {
  module_type: BrioInputSource
}

physics: {
  producers: {
    caloCD: {
      module_type: MockCalorimeterCalibrator
      tagLabel: "SD:calo"
      lowEnergyThreshold: 50
      highEnergyThreshold: 150
      timeWindow: 100
      caloModel: {
        energyResolution: 8
        alphaQuenchingParameters: [77.4, 0.639, 2.34]
        relaxationTime: 6.0
      }
    }

    xcaloCD: {
      module_type: MockCalorimeterCalibrator
      tagLabel: "SD:xcalo"
      lowEnergyThreshold: 50
      highEnergyThreshold: 150
      timeWindow: 100
      caloModel: {
        energyResolution: 12
        alphaQuenchingParameters: [77.4, 0.639, 2.34]
        relaxationTime: 6.0
      }
    }
  }

  analyzers: {
    calibrationAnalysis: {
      module_type: "MockCalibrationAnalyzer"
    }
  }

  productionPath: [ caloCD, xcaloCD ]
  analysisPath: [ calibrationAnalysis ]
}
```
To run it with a file output by the production `flsimulate`, simply do

```
$ ./flreconstruct -s MyFile.brio -c snemo_examples/CalibrationAnalysis.fcl
...
```

It should run and output a `MyCalibrationAnalysis.root` file in the directory in which you
ran the above command.

- TODOs:
  - Mock fcl scripts for "flsimulate", "flreconstruct"
  - Basic source/producer/analyser modules for known SNemo modules
    - These can just produce dummy/standard data for example
  - Basic brio-reader/translator for current flsimulate output.
  - Use of TFileService for analysis (though maybe have this above)
  - Mock up of data structures

Further Information
===================
- [FHiCL Syntax and Semantics](https://cdcvs.fnal.gov/redmine/documents/327)
- [Art Framework Parameters](https://cdcvs.fnal.gov/redmine/projects/art/wiki/ART_framework_parameters)
- [Overall Art Guide](https://cdcvs.fnal.gov/redmine/projects/art/wiki)
