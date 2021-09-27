# consumes Gemini3D as ExternalProject, providing imported target "gemini3d::gemini3d"
include(ExternalProject)

# target_link_libraries(... gemini3d::gemini3d)
# for user programs
add_library(gemini3d::gemini3d INTERFACE IMPORTED)

if(NOT GEMINI_ROOT)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(GEMINI_ROOT ${PROJECT_BINARY_DIR} CACHE PATH "default ROOT")
  else()
    set(GEMINI_ROOT ${CMAKE_INSTALL_PREFIX})
  endif()
endif()

find_package(MPI COMPONENTS C Fortran REQUIRED)
find_package(HWLOC)
find_package(HDF5 COMPONENTS Fortran)
find_package(ZLIB)
find_package(MUMPS)
find_package(SCALAPACK)
find_package(LAPACK)

set(GEMINI_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gemini3d${CMAKE_STATIC_LIBRARY_SUFFIX})

set(GEMINI_INCLUDE_DIRS ${GEMINI_ROOT}/include)

# libraries needed by Gemini3D
set(h5fortran_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}h5fortran${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(nc4fortran_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}nc4fortran${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(MSIS_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}msis00mod${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(GLOW_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glow${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(HWM_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}hwm_ifc${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(HWLOC_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}hwloc_ifc${CMAKE_STATIC_LIBRARY_SUFFIX}
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}hwloc_c${CMAKE_STATIC_LIBRARY_SUFFIX}
)

# these are available if Gemini3D auto-built them.
set(MUMPS_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}dmumps${CMAKE_STATIC_LIBRARY_SUFFIX}
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}mumps_common${CMAKE_STATIC_LIBRARY_SUFFIX}
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}pord${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(SCALAPACK_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}scalapack${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(LAPACK_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lapack${CMAKE_STATIC_LIBRARY_SUFFIX}
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}blas${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(HDF5_LIBRARIES)
if(WIN32)
  set(hdf5_names hdf5_hl_fortran hdf5_hl_f90cstub hdf5_fortran hdf5_f90cstub hdf5_hl hdf5)
else()
  set(hdf5_names hdf5_hl_fortran hdf5_fortran hdf5_hl hdf5)
endif()
foreach(_name ${hdf5_names})
  list(APPEND HDF5_LIBRARIES ${GEMINI_ROOT}/lib/lib${_name}${CMAKE_STATIC_LIBRARY_SUFFIX})
endforeach()

set(ZLIB_LIBRARIES ${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX})

# ExternalProject defined
set(gemini3d_byproducts
${GEMINI_LIBRARIES}
${nc4fortran_LIBRARIES}
${h5fortran_LIBRARIES}
${GLOW_LIBRARIES}
${MSIS_LIBRARIES}
${HWM_LIBRARIES}
${HWLOC_LIBRARIES}
)

if(NOT MUMPS_FOUND)
  list(APPEND gemini3d_byproducts ${MUMPS_LIBRARIES})
endif()

if(NOT SCALAPACK_FOUND)
  list(APPEND gemini3d_byproducts ${SCALAPACK_LIBRARIES})
endif()

if(NOT LAPACK_FOUND)
  list(APPEND gemini3d_byproducts ${LAPACK_LIBRARIES})
endif()

if(NOT HDF5_FOUND)
  list(APPEND gemini3d_byproducts ${HDF5_LIBRARIES})
endif()

if(NOT ZLIB_FOUND)
  list(APPEND gemini3d_byproducts ${ZLIB_LIBRARIES})
endif()

set(gemini3d_args
-DCMAKE_INSTALL_PREFIX:PATH=${GEMINI_ROOT}
-DBUILD_SHARED_LIBS:BOOL=false
-DCMAKE_BUILD_TYPE=Release
-DBUILD_TESTING:BOOL=false
)

ExternalProject_Add(GEMINI3D
GIT_REPOSITORY ${gemini3d_git}
GIT_TAG ${gemini3d_tag}
CMAKE_ARGS ${gemini3d_args}
BUILD_BYPRODUCTS ${gemini3d_byproducts}
INACTIVITY_TIMEOUT 15
UPDATE_DISCONNECTED true
CONFIGURE_HANDLED_BY_BUILD true)

file(MAKE_DIRECTORY ${GEMINI_INCLUDE_DIRS})
# avoid generate race condition

target_link_libraries(gemini3d::gemini3d INTERFACE ${gemini3d_byproducts})

if(MUMPS_FOUND)
  target_link_libraries(gemini3d::gemini3d INTERFACE MUMPS::MUMPS)
endif()

if(SCALAPACK_FOUND)
  #fix for homebrew installed scalapack
  if(NOT TARGET SCALAPACK::SCALAPACK AND TARGET scalapack)
    add_library(SCALAPACK::SCALAPACK INTERFACE IMPORTED)
    target_link_libraries(SCALAPACK::SCALAPACK INTERFACE scalapack)
  endif()
  target_link_libraries(gemini3d::gemini3d INTERFACE SCALAPACK::SCALAPACK)
endif()

if(LAPACK_FOUND)
  target_link_libraries(gemini3d::gemini3d INTERFACE LAPACK::LAPACK)
endif()

if(HDF5_FOUND)
  target_link_libraries(gemini3d::gemini3d INTERFACE HDF5::HDF5)
endif()

if(ZLIB_FOUND)
  target_link_libraries(gemini3d::gemini3d INTERFACE ZLIB::ZLIB)
endif()

if(HWLOC_FOUND)
  # these are distinct (in addition to) our own hwloc_ifc interface.
  target_link_libraries(gemini3d::gemini3d INTERFACE HWLOC::HWLOC)
endif()

target_link_libraries(gemini3d::gemini3d INTERFACE MPI::MPI_Fortran)

# libdl and libm are needed on some systems for HDF5
target_link_libraries(gemini3d::gemini3d INTERFACE ${CMAKE_DL_LIBS})

if(UNIX)
  target_link_libraries(gemini3d::gemini3d INTERFACE m)
endif(UNIX)

# for Fortran modules
target_include_directories(gemini3d::gemini3d INTERFACE ${GEMINI_INCLUDE_DIRS})

add_dependencies(gemini3d::gemini3d GEMINI3D)
