Specification of BRIO File Format(s)
====================================

Current SuperNEMO Falaise/Bayeux software writes/reads data to the so-called
"brio" file format. This is defined in the Bayeux library, and the specification
is split into two parts:

1. Main file format
2. Layout of file contents


Main File Format
================

Brio files are just plain [ROOT](https://root.cern.ch) v6 format files

Layout of File Contents
=======================

Brio files hold 0-N ROOT `TTree`(s (called "stores" in Brio). Each TTree has
one TBranch, storing one `brio_record` per entry. The `brio_record` structure
is organised as:

``` c++
class brio_record : public TObject {
public:
  TString    fSerialTag;  ///< The serialization tag of the data class
  UInt_t     fVersionTag; ///< The serialization version number of the data class
  TArrayCMod fDataBuffer; ///< The buffer of bytes that contains the Boost archive associated to the serialized data
  ...
  ClassDef(brio_record, 1)
};
```

and provides a buffer into which Boost.serialization binary data for the
type named in `fSerialTag` can be serialized. Thus, Brio is a bi-level
serialization scheme:

- ROOT is used to provide a basic file format and `TTree` based random
  access to per-event (or other) data in binary buffers.
- Boost.serialization is used to serialize objects bound to its system
  into a ROOT based binary buffer, with string based tracking of the
  typename.

This unusual and complex scheme is an artifact of history, but is _relatively_
straightforward to _read_ if the `TTree`/store names are known and their
expected content (event or otherwise) is known.


Expected Stores and Content for `flsimulate` output
===================================================
There are two stores:

1. "GI" holds metadata about the simulation that generated the file (provenance),
   modelled as 1-N `datatools::properties` instances.
2. "ER" holds the simulated events themselves, modeled as

Metadata model in "GI" Store
----------------------------
The "GI" store holds `datatools::properties`, each modelling a different
piece of metadata:

- TODO


Event model in "EI" Store
-------------------------
The "ER" store holds `datatools::things`, one per event. A `things` object is,
roughly, a map between `std::string` and concrete classes of `datatools::i_serializable`.
Each entry (colloquially "bank") in the map models a separate piece of event data,
and for `flsimulate` output, each event has two banks:

- "EH" (Event Header) Bank
  - Instance of `snemo::datamodel::event_header`
- "SD" (Simulated Data) Bank
  - Instance of `mctools::simulated_data`

These classes and their (sub)structure are described below.

High to Low Level C++ Data Model Classes
========================================

Note: Several classes have `datatools::properties` data members (often
named "auxilliaries"). These have been used to provide developers with
a way to add arbitrary data to an object (as long as it's `int`, `double`
`bool` `string` or collections of those). Going forward we want to remove
these as

- They are generally not used(!)
- The arbitrary nature of their interface is not suitable for a data model.
  Rather, this "post-it" note functionality should be modelled by associations,
  if required.

Some review will be needed to track down where/when "auxilliaries" are
filled/used to provide an interface in code.


`snemo::datamodel::event_header` Class
--------------------------------------
This class models event-global data. In pseudo-C++:

```c++
class snemo::datamodel::event_header
{
  enum generation_type {
    // real, mc, unknown
  };

  datatools::event_id         eid;
  generation_type             gtype;
  snemo::datamodel::timestamp time;
  // datatools::properties, general data, to be reviewed, expect to be ignored
};
```

`snemo::datamodel::timestamp` Class
-----------------------------------
This class models a picosecond precision timestamp

```c++
class snemo::datamodel::timestamp
{
  int64_t     _seconds_; ///< Number of seconds
  int64_t _picoseconds_; ///< Number of picoseconds
};
```


`mctools::simulated_data` Class
-------------------------------
TODO

Note: This class does use auxilliaries to store info...

`mctools::base_step_hit` Class
------------------------------
TODO

`datatools::event_id` Class
---------------------------
Models a run+event number

```c++
class datatools::event_id {
  int32_t run_number_;   ///< Run number
  int32_t event_number_; ///< Event number in run
};
```

`geomtools::base_hit` Class
---------------------------
General base class for hits

```c++
class geomtools::base_hit
{
  int32_t                    _hit_id_; ///< Unique hit ID
  geomtools::geom_id        _geom_id_; ///< Geometry ID
  datatools::properties _auxiliaries_; ///< Auxiliary properties (want to ignore, if possible)
};
```

`geomtools::vector_3d` Class
----------------------------
Models a three dimensional vector. A simple typedef to CLHEP's
vector type:

```c++
namespace geomtools {
  typedef CLHEP::Hep3Vector vector_3d;
};
```

`genbb::primary_event` Class
----------------------------
TODO

`genbb::primary_particle` Class
-------------------------------
TODO

Notes from code/`brio-dump`
===========================
Expect two banks:
 "EH" -> snemo::datamodel::event_header
         -> datatools::event_id
            -> int32_t (run number)
            -> int32_t (event number)
         -> snemo::datamodel::event_header::generation_type
            -> enum
         -> snemo::datamodel::timestamp
            -> int64_t (seconds)
            -> int64_t (picoseconds)
         -> datatools::properties
            -> ignore this (for now)
 "SD" -> mctools::simulated_data
         -> geomtools::vector_3d (common primary vertex, optional)
            -> this is a CLHEP vector
         -> double (common primary time, optional)
         -> genbb::primary_event (truth info)
            -> double (time, optional)
            -> geomtools::vector_3d (vertex, optional)
            -> std::list<genbb::primary_particle> (particles in event)
               -> genbb::primary_particle
                  -> int ("generation id")
                  -> int32_t ("extended Geant3 type of particle")
                  -> int32_t ("standard pdg code")
                  -> std::string ("particle string")
                  -> double (particle mass)
                  -> double (time)
                  -> geomtools::vector_3d (momentum)
                  -> geomtools::vector_3d (vertex)
                  -> datatools::properties (auxilliaries, can probably
                  ignore)
            -> std::string ("label")
            -> std::string ("classification", optional)
            -> double (event weight)
            -> datatools::properties (auxilliaries, probably can be
            ignored)
         -> int8_t (collection type, plain/handle step hits, in practice
         always have the handle type)
         -> std::map<std::string,
         std::vector<datatools::handle<base_step_hit> >
            -> Concentrate on base_step_hit (higher structures handled by
            Art)
            -> mctools::base_step_hit
               -> (inherits from) geomtools::base_hit
                   -> int32_t ("unique" hit id)
                   -> geomtools::geom_id (geometric address)
                   -> datatools::properties (auxilliaries, can ignore for
                   now)
               -> geomtools::vector_3d (step start position)
               -> geomtools::vector_3d (step end position)
               -> double (step start time)
               -> double (step end time)
               -> geomtools::vector_3d (step start momentum)
               -> geomtools::vector_3d (step end momentum)
               -> double (energy deposited along step)
               -> std::string (particle name that made step)
               -> double (biasing weight)
               ... some interfaces access the auxilliaries...
               is_primary_particle() true if step is by primary particle
               get_track_id() int
               get_parent_track_id() int


