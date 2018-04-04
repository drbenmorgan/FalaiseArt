FalaiseArt
==========

Experimental implementation of [Falaise](https://github.com/supernemo-dbd/falaise) using
the [art](https://art.fnal.gov) framework for data management and processing.


Installing Art
==============

1. Use Linux/Homebrew with the [art-suite tap](https://github.com/drbenmorgan/homebrew-art_suite)
2. Use Docker with the [art image](https://hub.docker.com/r/benmorgan/art/)
   ```
   $ docker run --rm -it benmorgan/art
   ```
2. From OpenScienceGrid CVMFS
   ```
   $ source /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setups
   $ setup art v2_10_03 -q e15:prof
   ```
   Note that at present this cannot be used to build the example plugins, but the
   [example scripts](example_scripts) can be run.

Getting Started
===============
Basic Art Usage
---------------
See [example_scripts](example_scripts) for a basic sanity check of your art installation
and working with FHiCL scripts for event/IO control.


Building the Falaise-Art Plugins
--------------------------------

Assuming you have Art installed as described above, Falaise-Art
follows a standard CMake-based build:

```console
... Assuming we start in the directory holding this README ...
$ mkdir build
$ cd build
$ cmake -DCMAKE_PREFIX_PATH=$(brew --prefix sqlite) ..
$ make && make test
```

Using Falaise-Art Plugins
-------------------------
See the [example Modules](FLArt/Examples) and the associated
[FHiCL scripts](fcl/examples).

Further Information
===================
- [FHiCL Syntax and Semantics](https://cdcvs.fnal.gov/redmine/documents/327)
- [Art Framework Parameters](https://cdcvs.fnal.gov/redmine/projects/art/wiki/ART_framework_parameters)
- [Overall Art Guide](https://cdcvs.fnal.gov/redmine/projects/art/wiki)
