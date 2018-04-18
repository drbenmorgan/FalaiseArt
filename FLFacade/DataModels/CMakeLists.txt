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

# 2. The art dictionary
#    Must link it with the interface layer
art_dictionary(DICTIONARY_LIBRARIES FLFacade_DataModels)
