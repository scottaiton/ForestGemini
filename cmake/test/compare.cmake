function(fortran_compare outdir refdir name)

add_test(NAME gemini:compare:${name}
COMMAND gemini3d.compare ${outdir} ${refdir})

set_tests_properties(gemini:compare:${name} PROPERTIES
TIMEOUT 60
FIXTURES_REQUIRED ${name}:run_fxt
RESOURCE_LOCK $<$<BOOL:${WIN32}>:cpu_mpi>
REQUIRED_FILES "${outdir}/inputs/config.nml;${refdir}/inputs/config.nml"
LABELS compare
)

# resource_lock compare for Windows, which can take 100x longer when run
# at same time with non-dependent sim runs.
# it's not a problem to run multiple compare at once, but it is a problem
# to run gemini3d.compare at same time as gemini.bin, even on different sims

endfunction(fortran_compare)


function(compare_gemini_output name outdir refdir)

fortran_compare(${outdir} ${refdir} ${name})

endfunction(compare_gemini_output)
