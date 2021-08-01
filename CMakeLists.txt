# -- fortran library
add_library(gemini_f OBJECT
  fortran_source/fc3d_gemini_bc3_default.f90 
  fortran_source/fc3d_gemini_block.f90 
)

target_include_directories(gemini_f INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)

# -- add library
add_library(gemini)
set_target_properties(gemini PROPERTIES EXPORT_NAME GEMINI)

target_sources(gemini PRIVATE 
  fc3d_gemini.cpp 
  fc3d_gemini_options.c
  $<TARGET_OBJECTS:gemini_f>
)

target_link_libraries(gemini PUBLIC clawpatch)

target_include_directories(gemini
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# -- install

install(FILES
	fc3d_clawpack46.h
	fc3d_clawpack46_options.h
	clawpack46_user_fort.h
	fc3d_clawpack46_fort.h 
  DESTINATION include
)

install(TARGETS gemini
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)


# -- imported target, for use from FetchContent
add_library(FORESTCLAW::GEMINI INTERFACE IMPORTED GLOBAL)
target_link_libraries(FORESTCLAW::GEMINI INTERFACE gemini)
