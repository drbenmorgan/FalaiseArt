# - Toplevel CMake script for FalaiseArt
cmake_minimum_required(VERSION 3.9)
project(FalaiseArt VERSION 0.1.0)

# - Cetbuildtools, and basic build env
find_package(cetbuildtools REQUIRED)
include(CetCMakeEnv)
cet_cmake_env()
cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  EXTRA_CXX_FLAGS -Wnon-virtual-dtor -Wdelete-non-virtual-dtor
  )



#-----------------------------------------------------------------------
# Dependencies
# Add everything that the product uses directly (likely not complete yet...)
#find_package(art 2.10 REQUIRED)
#find_package(canvas_root_io REQUIRED)
#find_package(ROOT 6 REQUIRED)

# Bayeux (may need Falaise later) for bridging
#find_package(Bayeux 3.0 REQUIRED)

# macros for art_dictionary and simple_plugin
#include(ArtDictionary)
#include(BuildPlugins)

#-----------------------------------------------------------------------
# Build what we need
#
#add_subdirectory(snemo)
#add_subdirectory(FLFacade)
