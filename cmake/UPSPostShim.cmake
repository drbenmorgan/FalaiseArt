# Only if we need UPS support
if(NOT FA_UPS_BUILD)
  return()
endif()

# Generate the cetpkg_variable_report file
# - Extract version from PROJECT_VERSION and UPS-ify
string(REPLACE "." "_" CETPKG_VERSION "${PROJECT_VERSION}")
set(CETPKG_VERSION "v${CETPKG_VERSION}")


file(WRITE ${PROJECT_BINARY_DIR}/cetpkg_variable_report
"
CETPKG_NAME             ${PROJECT_NAME}
CETPKG_VERSION          ${CETPKG_VERSION}
CETPKG_DEFAULT_VERSION  none
CETPKG_QUAL             ${CETPKG_QUAL}
CETPKG_TYPE             ${CETPKG_TYPE}
CETPKG_SOURCE           ${PROJECT_SOURCE_DIR}
CETPKG_BUILD            ${PROJECT_BINARY_DIR}
CETPKG_CC               ${CETPKG_CC}
CETPKG_CXX              ${CETPKG_CXX}
CETPKG_FC               ${CETPKG_FC}
CETPKG_ONLY_FOR_BUILD   cetbuildtools;

To check cmake cached variables, use cmake -N -L.
"
)

# Set default build type based on qualifier
if(NOT CMAKE_CONFIGURATION_TYPES)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${CETPKG_TYPE}" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Opt Prof." FORCE)
  endif()
endif()

