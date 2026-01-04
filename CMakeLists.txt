cmake_minimum_required(VERSION 3.15)
project(OSI2025_Sistem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB_RECURSE COMMON_SRC "src/common/*.cpp")
file(GLOB_RECURSE CLIENT_SRC "src/client/*.cpp")
file(GLOB_RECURSE EMPLOYEE_SRC "src/employee/*.cpp")

add_library(common_lib STATIC ${COMMON_SRC})
target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(KlijentApp ${CLIENT_SRC})
target_link_libraries(KlijentApp PRIVATE common_lib)

add_executable(ZaposleniApp ${EMPLOYEE_SRC})
target_link_libraries(ZaposleniApp PRIVATE common_lib)

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB TEST_SOURCES "tests/*.cpp")

    foreach(test_source ${TEST_SOURCES})
        get_filename_component(test_name ${test_source} NAME_WE)
        add_executable(${test_name} ${test_source})
        target_link_libraries(${test_name} PRIVATE common_lib GTest::gtest_main)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

install(TARGETS KlijentApp ZaposleniApp RUNTIME DESTINATION bin)