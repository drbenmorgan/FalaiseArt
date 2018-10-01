simple_plugin(GeometryService "service"
  Bayeux::Bayeux
  art_Framework_Principal
  )
simple_plugin(KernelService "service"
  Bayeux::Bayeux
  )
# As VariantService relies on KernelService, link to it
simple_plugin(VariantService "service"
  Bayeux::Bayeux
  snemo_services_KernelService_service
  )

simple_plugin(FooService "service"
  Bayeux::Bayeux
  snemo_services_KernelService_service
  )
