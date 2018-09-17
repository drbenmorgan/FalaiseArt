FalaiseArt
==========

Experimental implementation of [Falaise](https://github.com/supernemo-dbd/falaise) using
the [art](https://art.fnal.gov) framework for data management and processing.


Installing Art
==============

1. From OpenScienceGrid CVMFS. This assumes an install of CVMFS
   with configuration for the `larsoft.opensciencegrid.org` repo.

   ```
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
behaviour of Art and FHiCL with some simple examples. These are documented with
a basic walkthrough in the [README_examples.md](README_examples.md) file. Please
see that document for further information.


Building the FalaiseArt Plugins
--------------------------------

To build and use the FalaiseArt plugins, couple of additional steps are needed.
The initial setup of art is similar, but one additional package is required:

``` console
$ source <productbasedir>/setups
$ setup art v2_11_03 -q e17:debug
$ setup cetbuildtools v7_04_00
$ setup ninja v1_8_2
```

Art is setup as before, the additional `setup` adding the FNAL buildtools for compiling
UPS based code, and the `ninja` build tools. There is nothing special going on here apart
from the oddities of the UPS based install tree and environment setup. Ninja is used to
speed up the build over the default `make` system, especially when using a CVMFS
install of Art.

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

CMake will run as standard, and should warn you if any setup is missing. Simply type `ninja`
to build everything:

``` console
$ ninja
...
```

- Mock fcl scripts for "flsimulate", "flreconstruct"
- Basic source/producer/analyser modules for known SNemo modules
  - These can just produce dummy/standard data for example
- Basic brio-reader/translator for current flsimulate output.
- Use of TFileService for analysis (though maybe have this above)
- Mock up of data structures

Using FalaiseArt Plugins
-------------------------

Further Information
===================
- [FHiCL Syntax and Semantics](https://cdcvs.fnal.gov/redmine/documents/327)
- [Art Framework Parameters](https://cdcvs.fnal.gov/redmine/projects/art/wiki/ART_framework_parameters)
- [Overall Art Guide](https://cdcvs.fnal.gov/redmine/projects/art/wiki)
