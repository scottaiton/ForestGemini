
add_compile_options(
  $<$<Fortran_COMPILER_ID:Intel,IntelLLVM>:$<IF:$<BOOL:${WIN32}>,/QxHost,-xHost>>
  "$<$<COMPILE_LANG_AND_ID:Fortran,Intel,IntelLLVM>:-warn;-heap-arrays>"
  "$<$<AND:$<COMPILE_LANG_AND_ID:Fortran,Intel,IntelLLVM>,$<CONFIG:Debug>>:-traceback;-check;-debug>"
  "$<$<COMPILE_LANG_AND_ID:Fortran,GNU>:-mtune=native;-Wall;-fimplicit-none>"
  "$<$<AND:$<COMPILE_LANG_AND_ID:Fortran,GNU>,$<CONFIG:Debug>>:-Wextra;-fcheck=all;-Werror=array-bounds>"
  "$<$<AND:$<COMPILE_LANG_AND_ID:Fortran,GNU>,$<CONFIG:Release>>:-fno-backtrace;-Wno-maybe-uninitialized>"
  "$<$<AND:$<COMPILE_LANG_AND_ID:Fortran,GNU>,$<CONFIG:RelWithDebInfo>>:-Wno-maybe-uninitialized>"
  )
