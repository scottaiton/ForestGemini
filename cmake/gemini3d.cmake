# consumes Gemini3D as ExternalProject, providing imported target "gemini3d::gemini3d"
include(ExternalProject)

# target_link_libraries(... gemini3d::gemini3d)
# for user programs
add_library(gemini3d::gemini3d INTERFACE IMPORTED)

find_package(MPI COMPONENTS C Fortran REQUIRED)
find_package(HWLOC)

find_package(HDF5 COMPONENTS Fortran REQUIRED)

find_package(MUMPS COMPONENTS d REQUIRED)
find_package(SCALAPACK REQUIRED)
find_package(LAPACK REQUIRED)

# artifacts from ExternalProject GEM3D

add_executable(gemini3d.compare IMPORTED)
set_target_properties(gemini3d.compare PROPERTIES
IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/bin/gemini3d.compare
)

if(BUILD_SHARED_LIBS)
  if(WIN32)
    set(GEMINI_LIBRARIES ${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}gemini3d${CMAKE_SHARED_LIBRARY_SUFFIX})
  else()
    set(GEMINI_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}gemini3d${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
else()
  set(GEMINI_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gemini3d${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

set(GEMINI_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)

# ExternalProject defined

set(gemini3d_args
-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
-DBUILD_TESTING:BOOL=false
-Dautobuild:BOOL=false
-Drealbits=64
-Dglow:BOOL=${glow}
-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
)

string(JSON gemini3d_git GET ${lib_json} gemini3d git)
string(JSON gemini3d_tag GET ${lib_json} gemini3d tag)

ExternalProject_Add(GEMINI3D_RELEASE
GIT_REPOSITORY ${gemini3d_git}
GIT_TAG ${gemini3d_tag}
CMAKE_ARGS ${gemini3d_args} -DCMAKE_BUILD_TYPE=Release
CMAKE_GENERATOR ${EXTPROJ_GENERATOR}
BUILD_BYPRODUCTS ${GEMINI_LIBRARIES}
INACTIVITY_TIMEOUT 15
CONFIGURE_HANDLED_BY_BUILD true
)

# for Fortran modules
target_include_directories(gemini3d::gemini3d INTERFACE ${GEMINI_INCLUDE_DIRS})
file(MAKE_DIRECTORY ${GEMINI_INCLUDE_DIRS})
# avoid generate race condition

target_link_libraries(gemini3d::gemini3d INTERFACE ${GEMINI3D_LIBRARIES})

add_dependencies(gemini3d::gemini3d GEMINI3D_RELEASE)
