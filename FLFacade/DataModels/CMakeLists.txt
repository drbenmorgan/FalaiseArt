add_library(FLFacade_DataModels SHARED
  event_header.h
  event_header.ipp
  event_header.cc
  timestamp.h
  timestamp.ipp
  timestamp.cc
  bayeux_dict.cc
  )
target_include_directories(FLFacade_DataModels
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  )
target_link_libraries(FLFacade_DataModels
  PUBLIC
    Bayeux::Bayeux
  )

