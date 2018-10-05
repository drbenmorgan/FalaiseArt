# 0. Check whether we are using Homebrew or ups
find_program(FA_HAVE_HOMEBREW brew)
find_program(FA_HAVE_UPS ups)

# Having both is an erro
if(FA_HAVE_HOMEBREW AND FA_HAVE_UPS)
  message(FATAL_ERROR "Detected both homebrew and ups in your path at\n${FA_HAVE_HOMEBREW}\n${FA_HAVE_UPS}\n respectively. FalaiseArt can only be built against one.")
elseif(FA_HAVE_HOMEBREW)
  # No shimming required
  return()
endif()

set(FA_UPS_BUILD ON)

# 1. Check that we have an Art and Cetbuildtools setup
#    - These supply all needed dependencies
if(NOT DEFINED ENV{SETUP_ART})
  message(FATAL_ERROR "Art UPS product not setup")
endif()

if(NOT DEFINED ENV{SETUP_CETBUILDTOOLS})
  message(FATAL_ERROR "Cetbuildtools UPS product not setup")
endif()

# 2. Art's setup defines qualifier(s) which we need before
#    calling project() to set CC/CXX/FC
# Use FQ dir or Art to get compiler/build type info
get_filename_component(_project_fq_tail $ENV{ART_FQ_DIR} NAME)
# Compiler qual is ".(e|c)[0-9]+."
string(REGEX MATCH "\\.([ec][0-9]+)\\." _project_fq_qualifier "${_project_fq_tail}")
string(REPLACE "." "" _project_fq_qualifier "${_project_fq_qualifier}")
# We have to hack the env because UPS is pernickty about the
# exact compiler name..
if(_project_fq_qualifier MATCHES "^e[0-9]+")
  set(CETPKG_CC gcc)
  set(CETPKG_CXX g++)
  set(CETPKG_FC gfortran)
elseif(_project_fq_qualifier MATCHES "^c[0-9]+")
  set(CETPKG_CC clang)
  set(CETPKG_CXX clang++)
  set(CETPKG_FC gfortran)
else()
  message(FATAL_ERROR "Compiler/Standard '${_project_fq_qualifier}' unknown")
endif()

set(ENV{CC} ${CETPKG_CC})
set(ENV{CXX} ${CETPKG_CXX})
set(ENV{FC} ${CETPKG_FC})

# Build mode is ".(debug|opt|prof).?"
string(REGEX MATCH "\\.(debug|opt|prof)\\.?" _project_fq_build_type "${_project_fq_tail}")
string(REPLACE "." "" _project_fq_build_type "${_project_fq_build_type}")

# The CETPKG_TYPE variable seems only a transmission rather than fundamental
# variable, but we set it anyway
set(CETPKG_TYPE )
if(_project_fq_build_type MATCHES "debug")
  set(CETPKG_TYPE "Debug")
elseif(_project_fq_build_type MATCHES "opt")
  set(CETPKG_TYPE "Opt")
elseif(_project_fq_build_type MATCHES "prof")
  set(CETPKG_TYPE "Prof")
else()
  message(FATAL_ERROR "Build type '${_project_fq_build_type}' unknown")
endif()

# Overall qualifier
set(CETPKG_QUAL "${_project_fq_qualifier}:${_project_fq_build_type}")
