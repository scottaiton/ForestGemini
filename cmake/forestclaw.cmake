# consumes ForestClaw as ExternalProject, provided imported target forestclaw::forestclaw
include(ExternalProject)

# target_link_libraries(... forestclaw::forestclaw)
# for user programs
add_library(forestclaw::forestclaw INTERFACE IMPORTED)

if(NOT FCLAW_ROOT)
  set(FCLAW_ROOT ${CMAKE_INSTALL_PREFIX})
endif()

set(forestclaw_args
--install-prefix=${FCLAW_ROOT}
-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
-DCMAKE_BUILD_TYPE=Release
-DBUILD_TESTING:BOOL=false
)

ExternalProject_Add(FORESTCLAW
GIT_REPOSITORY ${forestclaw_git}
GIT_TAG ${forestclaw_tag}
CMAKE_ARGS ${forestclaw_args}
CMAKE_GENERATOR ${EXTPROJ_GENERATOR}
BUILD_BYPRODUCTS ${forestclaw_byproducts}
INACTIVITY_TIMEOUT 15
CONFIGURE_HANDLED_BY_BUILD true
)
