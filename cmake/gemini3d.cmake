# consumes Gemini3D as ExternalProject, providing imported target "gemini3d::gemini3d"
include(ExternalProject)

# target_link_libraries(... gemini3d::gemini3d)
# for user programs
add_library(gemini3d::gemini3d INTERFACE IMPORTED)

if(NOT GEMINI_ROOT)
  set(GEMINI_ROOT ${CMAKE_INSTALL_PREFIX})
endif()

find_package(MPI COMPONENTS C Fortran REQUIRED)
find_package(HWLOC)

find_package(HDF5 COMPONENTS Fortran REQUIRED)

find_package(MUMPS COMPONENTS d REQUIRED)
find_package(SCALAPACK REQUIRED)
find_package(LAPACK REQUIRED)

# artifacts from ExternalProject GEM3D

add_executable(gemini3d.compare IMPORTED)
set_target_properties(gemini3d.compare PROPERTIES
IMPORTED_LOCATION ${GEMINI_ROOT}/bin/gemini3d.compare
)

set(GEMINI_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_${lib_type}_LIBRARY_PREFIX}gemini3d${CMAKE_${lib_type}_LIBRARY_SUFFIX})

set(GEMINI_INCLUDE_DIRS ${GEMINI_ROOT}/include)

# libraries needed by Gemini3D
set(h5fortran_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_${lib_type}_LIBRARY_PREFIX}h5fortran${CMAKE_${lib_type}_LIBRARY_SUFFIX}
)

set(nc4fortran_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_${lib_type}_LIBRARY_PREFIX}nc4fortran${CMAKE_${lib_type}_LIBRARY_SUFFIX}
)

set(GLOW_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_${lib_type}_LIBRARY_PREFIX}glow${CMAKE_${lib_type}_LIBRARY_SUFFIX}
)

set(HWM_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_${lib_type}_LIBRARY_PREFIX}hwm_ifc${CMAKE_${lib_type}_LIBRARY_SUFFIX}
)

# ExternalProject defined
set(gemini3d_byproducts
${GEMINI_LIBRARIES}
${nc4fortran_LIBRARIES}
${h5fortran_LIBRARIES}
${MSIS_LIBRARIES}
${HWM_LIBRARIES}
)
if(glow)
  list(APPEND gemini3d_byproducts ${GLOW_LIBRARIES})
endif()

set(gemini3d_args
--install-prefix=${GEMINI_ROOT}
-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
-DBUILD_TESTING:BOOL=false
-Dautobuild:BOOL=off
-Drealbits=64
-Dglow:BOOL=${glow}
-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
)

ExternalProject_Add(GEMINI3D_RELEASE
GIT_REPOSITORY ${gemini3d_git}
GIT_TAG ${gemini3d_tag}
CMAKE_ARGS ${gemini3d_args} -DCMAKE_BUILD_TYPE=Release
CMAKE_GENERATOR ${EXTPROJ_GENERATOR}
BUILD_BYPRODUCTS ${gemini3d_byproducts}
INACTIVITY_TIMEOUT 15
CONFIGURE_HANDLED_BY_BUILD true
)

# for Fortran modules
target_include_directories(gemini3d::gemini3d INTERFACE ${GEMINI_INCLUDE_DIRS})
file(MAKE_DIRECTORY ${GEMINI_INCLUDE_DIRS})
# avoid generate race condition

target_link_libraries(gemini3d::gemini3d INTERFACE
${gemini3d_byproducts}
MUMPS::MUMPS
SCALAPACK::SCALAPACK
LAPACK::LAPACK
HDF5::HDF5
ZLIB::ZLIB
"$<$<BOOL:${HWLOC_FOUND}>:HWLOC::HWLOC>"
MPI::MPI_Fortran
${CMAKE_DL_LIBS}
$<$<BOOL:${UNIX}>:m>
)
# libdl and libm are needed on some systems for HDF5

add_dependencies(gemini3d::gemini3d GEMINI3D_RELEASE)
