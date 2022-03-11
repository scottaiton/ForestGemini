set(all ${PROJECT_SOURCE_DIR}/all)

add_library(simple_f OBJECT 
  setprob.f90
  user_4.6/qinit.f90 
  user_4.6/setaux.f90
)

add_executable(simple 
  simple_user.cpp 
  simple.cpp 
  simple_options.c
  $<TARGET_OBJECTS:simple_f>
)

target_include_directories(simple PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${all}
)

target_link_libraries(simple PRIVATE 
 fc3d_gemini
)

add_test(NAME examples_simple COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/regressions.sh WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_tests_properties(examples_simple PROPERTIES ENVIRONMENT "${FCLAW_TEST_ENVIRONMENT}")











