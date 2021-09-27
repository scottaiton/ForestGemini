cmake_minimum_required(VERSION 3.13...3.20)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

project(FC3DGEMINI LANGUAGES C CXX Fortran)

find_package(MPI)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(cmake/libraries.cmake)
include(cmake/compiler_flags.cmake)

# External Projects
include(cmake/gemini3d.cmake)
#include(cmake/forestclaw.cmake)
# --- get installed ForestClaw package
find_package(FORESTCLAW REQUIRED)
#add_subdirectory(gemini3d)
#include(cmake/libraries.cmake)
#include(cmake/gemini3d.cmake)

# -- fortran library
add_library(gemini_f OBJECT
  fortran_source/fc3d_gemini_bc3_default.f90 
  fortran_source/fc3d_gemini_block.f90 
)
target_include_directories(gemini_f INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)

# -- add library
add_library(fc3d_gemini)
set_target_properties(fc3d_gemini PROPERTIES EXPORT_NAME GEMINI)

target_sources(fc3d_gemini PRIVATE 
  fc3d_gemini.cpp 
  fc3d_gemini_options.c
  $<TARGET_OBJECTS:gemini_f>
)

target_link_libraries(fc3d_gemini PUBLIC  FORESTCLAW::CLAWPATCH FORESTCLAW::FORESTCLAW gemini3d::gemini3d)

target_include_directories(fc3d_gemini
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# -- imported target, for use from FetchContent
add_library(FORESTCLAW::GEMINI INTERFACE IMPORTED GLOBAL)
target_link_libraries(FORESTCLAW::GEMINI INTERFACE fc3d_gemini)

add_subdirectory(examples)
