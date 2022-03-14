# consumes ForestClaw as ExternalProject
include(ExternalProject)

add_library(FORESTCLAW::FORESTCLAW INTERFACE IMPORTED)
add_library(FORESTCLAW::CLAWPATCH INTERFACE IMPORTED)

if(BUILD_SHARED_LIBS)
  if(WIN32)
    set(FCLAW_LIBRARIES ${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}forestclaw${CMAKE_SHARED_LIBRARY_SUFFIX})
  else()
    set(FCLAW_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}forestclaw${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
else()
  set(FCLAW_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}forestclaw${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

set(FCLAW_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
file(MAKE_DIRECTORY ${FCLAW_INCLUDE_DIRS})
target_include_directories(FORESTCLAW::FORESTCLAW INTERFACE ${FCLAW_INCLUDE_DIRS})

set(forestclaw_args
-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
-DCMAKE_BUILD_TYPE=Release
-DBUILD_TESTING:BOOL=false
-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
-Dmpi:BOOL=yes
)

string(JSON forestclaw_git GET ${lib_json} forestclaw git)
string(JSON forestclaw_tag GET ${lib_json} forestclaw tag)

ExternalProject_Add(FORESTCLAW
GIT_REPOSITORY ${forestclaw_git}
GIT_TAG ${forestclaw_tag}
CMAKE_ARGS ${forestclaw_args}
CMAKE_GENERATOR ${EXTPROJ_GENERATOR}
BUILD_BYPRODUCTS ${FCLAW_LIBRARIES}
INACTIVITY_TIMEOUT 15
CONFIGURE_HANDLED_BY_BUILD true
)

add_dependencies(FORESTCLAW::FORESTCLAW FORESTCLAW)
add_dependencies(FORESTCLAW::CLAWPATCH FORESTCLAW)
