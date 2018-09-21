add_library(snemo_datamodel_bridge SHARED
  event_header.h
  event_header.ipp
  event_header.cc
  timestamp.h
  timestamp.ipp
  timestamp.cc
  bayeux_dict.cc
  )
target_include_directories(snemo_datamodel_bridge
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  )
target_link_libraries(snemo_datamodel_bridge
  PUBLIC
    Bayeux::Bayeux
  )

# TBR: May not need a dictionary if we simply translate
# 2. The art dictionary
#    Must link it with the interface layer
#art_dictionary(DICTIONARY_LIBRARIES FLFacade_DataModels)
