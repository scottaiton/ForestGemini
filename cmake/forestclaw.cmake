# consumes ForestClaw as ExternalProject, provided imported target forestclaw::forestclaw
include(ExternalProject)

# target_link_libraries(... forestclaw::forestclaw)
# for user programs
add_library(forestclaw::forestclaw INTERFACE IMPORTED)

if(NOT FCLAW_ROOT)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(FCLAW_ROOT ${PROJECT_BINARY_DIR} CACHE PATH "default ForestClaw ROOT")
  else()
    set(FCLAW_ROOT ${CMAKE_INSTALL_PREFIX})
  endif()
endif()

set(forestclaw_args
-DCMAKE_INSTALL_PREFIX:PATH=${FCLAW_ROOT}
-DBUILD_SHARED_LIBS:BOOL=false
-DCMAKE_BUILD_TYPE=Release
-DBUILD_TESTING:BOOL=false
)

ExternalProject_Add(FORESTCLAW
GIT_REPOSITORY ${forestclaw_git}
GIT_TAG ${forestclaw_tag}
CMAKE_ARGS ${forestclaw_args}
BUILD_BYPRODUCTS ${forestclaw_byproducts}
INACTIVITY_TIMEOUT 15
UPDATE_DISCONNECTED true
CONFIGURE_HANDLED_BY_BUILD true)
