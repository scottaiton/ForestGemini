cmake_minimum_required(VERSION 3.20)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

project(FC3DGEMINI LANGUAGES C CXX Fortran)

find_package(MPI)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(cmake/libraries.cmake)
include(cmake/compiler_flags.cmake)

# libraries
find_package(gemini3d)
find_package(FORESTCLAW REQUIRED)

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

target_include_directories(fc3d_gemini
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)

add_subdirectory(examples)
