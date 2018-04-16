FLFacade
========

Libraries and programs to provide a facade/bridge to Falaise/Bayeux
core data files, types and singleton interfaces.

Programs
========
- `brio_dump`
  - Simple command line program to dump contents of a brio file.


Sublibraries
============
FLDataModels
------------
Needed data models over and above core Bayeux to read base Falaise files
output by `flsimulate` (and later the DAQ). These require a Boost serialization
dictionary generated to read them.

