set(_lib_names forestclaw gemini3d iniparser)

file(READ ${CMAKE_CURRENT_LIST_DIR}/libraries.json _libj)

foreach(n ${_lib_names})
  foreach(t git tag)
    string(JSON m GET ${_libj} ${n} ${t})
    set(${n}_${t} ${m})
  endforeach()
endforeach()
