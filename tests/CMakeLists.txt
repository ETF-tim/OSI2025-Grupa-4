cmake_minimum_required(VERSION 3.10)
project(tests)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_bill test_bill.cpp)
target_link_libraries(test_bill ${GTEST_LIBRARIES} pthread)

add_executable(test_hello_world test_hello_world.cpp)
target_link_libraries(test_hello_world ${GTEST_LIBRARIES} pthread)

add_test(NAME test_bill COMMAND test_bill)
add_test(NAME test_hello_world COMMAND test_hello_world)