TRIBITS_ADD_ADVANCED_TEST( cts1empire_custom_builds_unit_tests
  OVERALL_WORKING_DIRECTORY TEST_NAME
  OVERALL_NUM_MPI_PROCS 1
  TEST_0 CMND env
    ARGS SHUNIT_COLOR=none ${CMAKE_CURRENT_SOURCE_DIR}/custom_builds_unit_tests.sh
    PASS_REGULAR_EXPRESSION_ALL
      "ERROR: A supported compiler was not selected for 'cts1empire' env - anything-intell"
      "Ran 1 test"
      "[^]OK[$]"
    ALWAYS_FAIL_ON_NONZERO_RETURN
  )
