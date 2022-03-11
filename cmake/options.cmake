option(dev "development mode")
option(glow "NCAR GLOW model" true)


if(dev)
else()
  set_directory_properties(PROPERTIES EP_UPDATE_DISCONNECTED true)
endif()

# --- naming shared/static
set(lib_type STATIC)
if(BUILD_SHARED_LIBS)
  set(lib_type SHARED)
endif()

# --- External project generator
if(CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
  set(EXTPROJ_GENERATOR "Ninja")
else()
  set(EXTPROJ_GENERATOR ${CMAKE_GENERATOR})
endif()

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

# --- default install directory under build/local
# users can specify like "cmake -B build --install-prefix=~/mydir"
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  # will not take effect without FORCE
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "Install top-level directory" FORCE)
endif()
