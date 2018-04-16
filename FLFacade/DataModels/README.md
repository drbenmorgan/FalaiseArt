FLDataModels
============

Minimal data models to bridge to flsimulate files:

- `snemo::datamodels::timestamp`
- `snemo::datamodels::event_header`

Creating a Bayeux/Boost "Dictionary"
------------------------------------
1. Class Header must `#include`
   - `boost/serialization/access.hpp`
   - `boost/serialization/version.hpp`
   - `bayeux/datatools/i_serializable.h`
2. Class must inherit from `datatools::i_serializable`
3. Class declaration must put macro `DATATOOLS_SERIALIZATION_DECLARATION()`
   at end of final `private` section
4. Class Header must have macro `BOOST_CLASS_VERSION(<fqclassname>, <versionint>)`
   after declaration.
5. Class Implementation file must have macro `DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(classname,"<fqclassname>")`
   before further definitions, and inside class's namespace.
6. A `.ipp` file must be created for the class being serialized. If the
   class has data members that are concrete classes of `i_serializable`
   then their `.ipp` headers must be `#include`d at the top of the file.
   The `.ipp` file
   must define the `serialize` member function for the class, This is a
   template method on the Boost Archive type. It takes a reference to the
   archive and integer for the class version. You must add entries in the archive for the class data members
   you wish to persist. If you are using schema evolution, you must use the
   supplied integer for the version to handle any changes in the schema.
7. The `.ipp` file must be `#include`d in the `bayeux_dict.cc` file... TODO



