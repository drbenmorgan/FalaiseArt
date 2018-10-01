FalaiseArt Services
===================

Falaise's underlying Bayeux library provides services. Whilst many of
these are easily convertible to Art services, there are a few oddities.
These are due to the use of the `datatools::kernel` class, which provides
a second communication channel between a few services.

For example, the `datatools::variant_service` registers itself with the
kernel as part of its startup. The kernel (singleton, BTW) has member
functions to return both the service, or things it holds...

- TBD: where these functions are called behind the scenes in Bayeux.
  In principle, everywhere kernel is called. Appears confined to
  datatools, or highlevel programs only.
- The major one in terms of usability of Bayeux is use `variant_preprocessor`
  in `datatools::properties`. That affects parsing and use of config files

The `datatools::kernel` also holds its own `datatools::service_manager`, which
holds a `datatools::library_query_service` and a `datatools::urn_query_service`.
There're no direct "set" member functions for these, but there are backdoor
registrations via the kernel singleton. The `urn_query_service` is just returned
directly. The `library_query_service` is also return, as is a `library_info` object
which is obtained from the service.

The `library_query_service` is used in Falaise to register resource paths and so on.
These are likely used to resolve paths in the form "@falaise:<path>". Yes, used
in implementation of `datatools/utils.h". Could provide an Art service to wrap these,
but behind the scenes stuff in Bayeux would still need to use the kernel.

For the `datatools::urn_query_service`, it's constructed in `datatools::kernel::_initialize_urn_query_service_()`
as

```c++
datatools::urn_query_service & kUrnQuery = dynamic_cast<datatools::urn_query_service &>(_services_->load_no_init("bxDtKernUrnQuery","datatools::urn_query_service"));
kUrnQuery.initialize_simple();
```

that member function is always called as part of singleton init. The resulting
service is handed out via the `get/grab_urn_query` member functions. These are
used in Bayeux in two places:

- `datatools::urn_to_path_resolver_service`
  - Via the `initialize` and `kernel_push` member functions, the service adds itself
    to the kernels `urn_query_service` IAOI the properties config contains a "kernel.push"
    flag.
- `datatools::urn_db_service`
  - As above

In Falaise, both of these services are setup and registered with the kernel in the
`falaise::falaise_sys` singleton, specifically the `_initialize_urn_services_`
member function.


Suggests that we need a "Bayeux/FalaiseKernelService" that offers no interface and simply
exists to ensure the singleton is started. Other services can then depend on this, ensuring
that the singleton is available. However, may be some dependencies on other areas such
as `libinfo` and so on.

FalaiseArt Services
===================
Couple of important things:

1. We can make a service depend on others by holding an art::ServiceHandle to the
other service, obtaining it in the constructor.
2. A dependent service must still be declared in the FHiCL `services` table even if we
don't use it directly. Art does emit a helpful error message of we forget it.
3. Services are constructed in dependency, then alphanumeric order.
4. Services are destructed in reverse order to construction.
5. At least the preBeginRun service callbacks are called for each service in
   service construction order (not clear for post actions...)

Will need to consider these, as Art introduces the Run->Run transition, which
we don't have in Falaise.

In some cases, the kernel layer requires actual Bayeux services, so may need to
hold instances of these in the Art "wrapper" service rather than the object
the Bayeux service wraps (e.g. geometry manager in geometry service). It should
still be possible to write suitable interfaces, just with one extra layer of
indirection.

