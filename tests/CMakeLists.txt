add_executable(unit_tests test_main.cpp)

target_link_libraries(unit_tests 
    GTest::gtest_main 
    LogikaProjekta
)

include(GoogleTest)
gtest_discover_tests(unit_tests)