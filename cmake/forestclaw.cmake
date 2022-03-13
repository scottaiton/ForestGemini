# consumes ForestClaw as ExternalProject, provided imported target forestclaw::forestclaw
include(ExternalProject)

# target_link_libraries(... forestclaw::forestclaw)
# for user programs
add_library(forestclaw::forestclaw INTERFACE IMPORTED)


set(forestclaw_args
-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
-DCMAKE_BUILD_TYPE=Release
-DBUILD_TESTING:BOOL=false
-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
)

string(JSON forestclaw_git GET ${lib_json} forestclaw git)
string(JSON forestclaw_tag GET ${lib_json} forestclaw tag)

ExternalProject_Add(FORESTCLAW
GIT_REPOSITORY ${forestclaw_git}
GIT_TAG ${forestclaw_tag}
CMAKE_ARGS ${forestclaw_args}
CMAKE_GENERATOR ${EXTPROJ_GENERATOR}
BUILD_BYPRODUCTS ${forestclaw_byproducts}
INACTIVITY_TIMEOUT 15
CONFIGURE_HANDLED_BY_BUILD true
)
