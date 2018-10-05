# - Toplevel CMake script for FalaiseArt
cmake_minimum_required(VERSION 3.9)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/UPSPreShim.cmake)
project(FalaiseArt VERSION 0.1.0)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/UPSPostShim.cmake)

# - Cetbuildtools, and basic build env
if(FA_UPS_BUILD)
  find_package(cetbuildtools REQUIRED)
  include(CetCMakeEnv)
  cet_cmake_env()
  cet_set_compiler_flags(DIAGS VIGILANT
    WERROR
    NO_UNDEFINED
    EXTRA_FLAGS -pedantic
    EXTRA_CXX_FLAGS -Wnon-virtual-dtor -Wdelete-non-virtual-dtor
    )
else()
  find_package(cetbuildtools2 0.6.0 REQUIRED)
  set(CMAKE_MODULE_PATH ${cetbuildtools2_MODULE_PATH})
  set(CET_COMPILER_CXX_STANDARD_MINIMUM 14)
  include(CetInstallDirs)
  include(CetCMakeSettings)
  include(CetCompilerSettings)
  include_directories(${PROJECT_SOURCE_DIR})
endif()

# Binreloc generator for anything needing selflocation
include(${PROJECT_SOURCE_DIR}/cmake/binreloc/GenerateBinreloc.cmake)

#-----------------------------------------------------------------------
# Dependencies
# Add everything that the product uses directly (likely not complete yet...)
find_package(art 2.11 REQUIRED)
# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(BuildPlugins)

find_package(CLHEP REQUIRED)
find_package(fhiclcpp REQUIRED)
find_package(ROOT REQUIRED)

# - Falaise deps used via ExternalProject. This include is essentially
#  find_package(Bayeux), and enables relevant imported targets
if(FA_UPS_BUILD)
  include(snemo_externals/snemo_externals.cmake)
else()
  find_package(Bayeux REQUIRED)
endif()

# Alias targets for Homebrew/UPS compatibility
add_library(CLHEP INTERFACE)
target_link_libraries(CLHEP INTERFACE $<IF:$<BOOL:${FA_UPS_BUILD}>,${CLHEP},CLHEP::CLHEP>)

add_library(fhiclcpp INTERFACE)
target_link_libraries(fhiclcpp INTERFACE $<IF:$<BOOL:${FA_UPS_BUILD}>,${FHICLCPP},fhiclcpp::fhiclcpp>)

if(NOT FA_UPS_BUILD)
  foreach(_artLib
      art_Framework_Principal
      art_Framework_Services_Registry
      art_Framework_Services_Optional
      art_Framework_Services_Optional_TFileService_service)
    add_library(${_artLib} INTERFACE)
    target_link_libraries(${_artLib} INTERFACE art::${_artLib})
  endforeach()
endif()

#-----------------------------------------------------------------------
# Build what we need
# - Test linkage
add_executable(testLinkage testLinkage.cc)
target_link_libraries(testLinkage PRIVATE Bayeux::Bayeux)

add_subdirectory(snemo)
add_subdirectory(programs)
add_subdirectory(fcl)

#add_subdirectory(cpp_examples)


