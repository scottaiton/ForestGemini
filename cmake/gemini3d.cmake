# consumes Gemini3D as ExternalProject
include(ExternalProject)

if(NOT GEMINI_ROOT)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(GEMINI_ROOT ${PROJECT_BINARY_DIR} CACHE PATH "default ROOT")
  else()
    set(GEMINI_ROOT ${CMAKE_INSTALL_PREFIX})
  endif()
endif()

set(GEMINI_LIBRARIES
${GEMINI_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gemini${CMAKE_STATIC_LIBRARY_SUFFIX})

set(GEMINI_INCLUDE_DIRS ${GEMINI_ROOT}/include)

ExternalProject_Add(GEMINI3D
GIT_REPOSITORY ${gemini3d_git}
GIT_TAG ${gemini3d_tag}
CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${GEMINI_ROOT} -DBUILD_SHARED_LIBS:BOOL=false -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING:BOOL=false
BUILD_BYPRODUCTS ${GEMINI_LIBRARIES}
INACTIVITY_TIMEOUT 15
CONFIGURE_HANDLED_BY_BUILD true)

file(MAKE_DIRECTORY ${GEMINI_INCLUDE_DIRS})
# avoid generate race condition

add_library(gemini3d::gemini3d INTERFACE IMPORTED)
target_link_libraries(gemini3d::gemini3d INTERFACE ${GEMINI_LIBRARIES})
target_include_directories(gemini3d::gemini3d INTERFACE ${GEMINI_INCLUDE_DIRS})

add_dependencies(gemini3d::gemini3d GEMINI3D)
