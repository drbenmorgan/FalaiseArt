set(EXAMPLES_TEST_PASS_REGEX "Art has completed and will exit with status 0.")

macro(run_art_test _name _fhicl)
  add_test(NAME ${_name} COMMAND art -c ${_fhicl})
  set_tests_properties(${_name} PROPERTIES
    PASS_REGULAR_EXPRESSION "${EXAMPLES_TEST_PASS_REGEX}"
    ENVIRONMENT "FHICL_FILE_PATH=${CMAKE_CURRENT_SOURCE_DIR}:."
    )
endmacro()

run_art_test(Example_Zero examples/zero.fcl)
run_art_test(Example_First examples/first.fcl)
run_art_test(Example_Second examples/second.fcl)
run_art_test(Example_Third examples/third.fcl)
run_art_test(Example_Fourth examples/fourth.fcl)
set_tests_properties(Example_Fourth PROPERTIES DEPENDS Example_Third)
run_art_test(Example_Fifth examples/fifth.fcl)
set_tests_properties(Example_Fourth PROPERTIES DEPENDS Example_Third)
run_art_test(Example_Sixth examples/sixth.fcl)
run_art_test(Example_Seventh examples/seventh.fcl)
set_tests_properties(Example_Seventh PROPERTIES DEPENDS Example_Third)
run_art_test(Example_Eighth examples/eighth.fcl)
run_art_test(Example_Ninth examples/ninth.fcl)
set_tests_properties(Example_Ninth PROPERTIES WILL_FAIL TRUE)



