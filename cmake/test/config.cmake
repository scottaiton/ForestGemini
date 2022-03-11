include(${CMAKE_CURRENT_LIST_DIR}/compare.cmake)


function(setup_gemini_test name TIMEOUT)

# --- setup test
cmake_path(APPEND out_dir ${PROJECT_BINARY_DIR} ${name})
cmake_path(APPEND ref_root ${PROJECT_SOURCE_DIR} test_data/compare)
cmake_path(APPEND ref_dir ${ref_root} ${name})

add_test(NAME ${name}:download
COMMAND ${CMAKE_COMMAND} -Dname=${name} -Doutdir:PATH=${out_dir} -Drefroot:PATH=${ref_root} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/download.cmake
)
set_tests_properties(${name}:download PROPERTIES
FIXTURES_SETUP ${name}:download_fxt
RESOURCE_LOCK download_lock  # avoid anti-leeching transient failures
LABELS download
TIMEOUT 180
)

# TODO: construct command
# set(test_cmd gemini3d_c.run ${out_dir} -exe $<TARGET_FILE:gemini_c.bin>)
set(test_cmd ${MPIEXEC_EXECUTABLE} -n 2 $<TARGET_FILE:gemini_c.bin> ${out_dir})

# TODO frontend C
# list(APPEND test_cmd -mpiexec ${MPIEXEC_EXECUTABLE})


add_test(NAME gemini:${name}:dryrun
COMMAND ${test_cmd} -dryrun
)

set_tests_properties(gemini:${name}:dryrun PROPERTIES
TIMEOUT 60
RESOURCE_LOCK cpu_mpi
FIXTURES_REQUIRED "gemini_exe_fxt;${name}:download_fxt"
FIXTURES_SETUP ${name}:dryrun
REQUIRED_FILES ${out_dir}/inputs/config.nml
LABELS core
)


add_test(NAME gemini:${name} COMMAND ${test_cmd})

set_tests_properties(gemini:${name} PROPERTIES
TIMEOUT ${TIMEOUT}
RESOURCE_LOCK cpu_mpi
FIXTURES_REQUIRED ${name}:dryrun
FIXTURES_SETUP ${name}:run_fxt
LABELS core
)

compare_gemini_output(${name} ${out_dir} ${ref_dir})

endfunction(setup_gemini_test)
