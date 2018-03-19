# A data products library usually provides
# 1. A pure C++ interface layer
# 2. The dictionary layer for interfacing with ROOT/Art

# 1. Here's the C++ interface layer, just an INTERFACE
#    library because we have "dumb" structs
add_library(FLArt_RawDataProducts INTERFACE)
target_include_directories(FLArt_RawDataProducts INTERFACE ${PROJECT_SOURCE_DIR})

# 2. The art dictionary
#    Must link it with the interface layer
art_dictionary(DICTIONARY_LIBRARIES FLArt_RawDataProducts)
