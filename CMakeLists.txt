# - Toplevel CMake script for FalaiseArt
cmake_minimum_required(VERSION 3.9)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/UPSPreShim.cmake)
project(FalaiseArt VERSION 0.1.0)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/UPSPostShim.cmake)

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
find_package(art 2.11 REQUIRED)
# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(BuildPlugins)


# - Falaise deps used via ExternalProject. This include is essentially
#  find_package(Bayeux), and enables relevant imported targets
include(snemo_externals/snemo_externals.cmake)

#-----------------------------------------------------------------------
# Build what we need
# - Test linkage
add_executable(testLinkage testLinkage.cc)
target_link_libraries(testLinkage PRIVATE Bayeux::Bayeux)

add_subdirectory(snemo)

add_subdirectory(programs)

add_subdirectory(fcl)
