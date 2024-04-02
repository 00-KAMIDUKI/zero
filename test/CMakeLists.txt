function(make_test name cxx_standard)
  add_executable(${name} ${name}.cc)
  target_link_libraries(${name} PRIVATE ${PROJECT_NAME})
  set_target_properties(${name} PROPERTIES
    CXX_STANDARD ${cxx_standard}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  add_test(NAME ${name} COMMAND ${name})
endfunction()

function(make_tests_for_standard cxx_standard)
  file(GLOB_RECURSE sources CONFIGURE_DEPENDS *_cxx${cxx_standard}.cc)
  foreach(source ${sources})
    get_filename_component(name ${source} NAME_WE)
    make_test(${name} ${cxx_standard})
  endforeach()
endfunction()

make_tests_for_standard(17)
make_tests_for_standard(20)

