# - Toplevel CMake script for FalaiseArt
cmake_minimum_required(VERSION 3.3)
project(FalaiseArt VERSION 0.0.1)

# - Cetbuildtools, version2
find_package(cetbuildtools2 0.6.0 REQUIRED)
set(CMAKE_MODULE_PATH ${cetbuildtools2_MODULE_PATH})
set(CET_COMPILER_CXX_STANDARD_MINIMUM 14)
include(CetInstallDirs)
include(CetCMakeSettings)
include(CetCompilerSettings)

#-----------------------------------------------------------------------
# Dependencies
# Add everything that the product uses directly (likely not complete yet...)
find_package(art 2.10 REQUIRED)
find_package(canvas_root_io REQUIRED)
find_package(ROOT 6 REQUIRED)

# Bayeux (may need Falaise later) for bridging
find_package(Bayeux 3.0 REQUIRED)

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(BuildPlugins)

#-----------------------------------------------------------------------
# Build what we need
#
add_subdirectory(snemo)
add_subdirectory(FLFacade)
