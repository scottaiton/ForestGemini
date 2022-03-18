cmake_minimum_required(VERSION 3.20...3.23)

project(FC3DGEMINI
LANGUAGES C CXX Fortran
)

find_package(MPI COMPONENTS C Fortran REQUIRED)

enable_testing()

cmake_path(SET CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(cmake/options.cmake)
include(cmake/compiler_flags.cmake)

# Gemini3D
# WORKAROUND: until Gemini3D package updated, we find packages here.
# the best way to do this is with find_dependency() calls in gemini3d:cmake/config.cmake.in
set(arith d)
find_package(HWLOC REQUIRED)
find_package(LAPACK REQUIRED)
find_package(SCALAPACK REQUIRED)
find_package(MUMPS COMPONENTS ${arith} REQUIRED)
find_package(HDF5 COMPONENTS C Fortran REQUIRED)
find_package(h5fortran CONFIG REQUIRED)
find_package(nc4fortran CONFIG REQUIRED) # dummy stub installed also from Gemini3d (or real nc4fortran)
find_package(ffilesystem CONFIG REQUIRED)
find_package(msis CONFIG REQUIRED)
find_package(glow CONFIG REQUIRED)
find_package(hwm14 CONFIG)
find_package(gemini3d CONFIG REQUIRED)

# ForestClaw: build as ExternalProject
cmake_path(SET CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ForestClawExternalProject)
include(ForestClawExternalProject)
ForestClawExternalProject(TAG develop COMPONENTS mpi clawpack)

# -- fortran library : dummy library containing .o files compiled from Fortran
add_library(gemini_f OBJECT
fortran_source/fc3d_gemini_bc3_default.f90
fortran_source/fc3d_gemini_block.f90
)
target_include_directories(gemini_f INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)

# -- add library : fc3d_gemini
add_library(fc3d_gemini)
set_target_properties(fc3d_gemini PROPERTIES EXPORT_NAME GEMINI)

target_sources(fc3d_gemini PRIVATE
src/fc3d_gemini.cpp
src/fc3d_gemini_options.c
$<TARGET_OBJECTS:gemini_f>
)

target_link_libraries(fc3d_gemini PUBLIC FORESTCLAW::CLAWPATCH FORESTCLAW::FORESTCLAW)
target_link_libraries(fc3d_gemini PUBLIC gemini3d::gemini3d)

target_include_directories(fc3d_gemini PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
$<INSTALL_INTERFACE:include>
)

add_subdirectory(examples)
