include(CetTest)
cet_test_env()

add_executable(fhicl-id fhicl-id.cc)
target_link_libraries(fhicl-id PRIVATE fhiclcpp)

# Comparisons
foreach(_p 1 2 3 4 5 6 7 8 9 10 11)
  add_test(NAME fhiclid-basic-order${_p} COMMAND fhicl-id basic_perm0.fcl basic_perm${_p}.fcl)
  set_tests_properties(fhiclid-basic-order${_p} PROPERTIES ENVIRONMENT "FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/fcl")
endforeach()

# Permutations that fail are 5,
set_tests_properties(fhiclid-basic-order5 PROPERTIES WILL_FAIL TRUE)
