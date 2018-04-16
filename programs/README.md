BRIO Portability tests
======================

- Compile brio_dump against current C++11/Root6.08 Bayeux version
  - If it cannot read validation files, then ROOT/C++/Boost issue
  - If it can, then possible Falaise types in store
- Compile brio_portability_test against current C++/Root6.08 Bayeux
  - Use write mode to generate pba and xml files
  - Copy to laptop, try reading with C++14/ROOT/Boost
  - See what happens.
