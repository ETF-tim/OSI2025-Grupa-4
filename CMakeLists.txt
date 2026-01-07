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
file(GLOB_RECURSE CLIENT_MODULE_SRC "src/client/*.cpp")
file(GLOB_RECURSE EMPLOYEE_MODULE_SRC "src/employee/*.cpp")

# Debug: Print what files were found
message(STATUS "COMMON_SRC files found: ${COMMON_SRC}")
message(STATUS "CLIENT_MODULE_SRC files found: ${CLIENT_MODULE_SRC}")
message(STATUS "EMPLOYEE_MODULE_SRC files found: ${EMPLOYEE_MODULE_SRC}")

# Common library - shared by all modules
if(COMMON_SRC)
    add_library(common_lib STATIC ${COMMON_SRC})
    target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
else()
    message(WARNING "No source files found in src/common/. Creating interface library.")
    add_library(common_lib INTERFACE)
    target_include_directories(common_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

# Employee module library
if(EMPLOYEE_MODULE_SRC)
    add_library(employee_lib STATIC ${EMPLOYEE_MODULE_SRC})
    target_include_directories(employee_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(employee_lib PUBLIC common_lib)
else()
    message(WARNING "No source files found in src/employee/")
endif()

# Client module library
if(CLIENT_MODULE_SRC)
    add_library(client_lib STATIC ${CLIENT_MODULE_SRC})
    target_include_directories(client_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(client_lib PUBLIC common_lib)
else()
    message(WARNING "No source files found in src/client/")
endif()

# Executables
if(TARGET employee_lib)
    add_executable(ZaposleniApp src/mainEmployee.cpp)
    target_link_libraries(ZaposleniApp PRIVATE employee_lib common_lib)
else()
    message(WARNING "Cannot create ZaposleniApp - employee_lib not available")
endif()

if(TARGET client_lib)
    add_executable(KlijentApp src/mainClient.cpp)
    target_link_libraries(KlijentApp PRIVATE client_lib common_lib)
else()
    message(WARNING "Cannot create KlijentApp - client_lib not available")
endif()

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
    
    message(STATUS "TEST_SOURCES files found: ${TEST_SOURCES}")

    if(TEST_SOURCES)
        foreach(test_source ${TEST_SOURCES})
            get_filename_component(test_name ${test_source} NAME_WE)
            add_executable(${test_name} ${test_source})
            
            if(test_name MATCHES ".*[Uu]sers.*" OR test_name MATCHES ".*[Ee]mployee.*" OR test_name MATCHES ".*[Zz]aposleni.*")
                if(TARGET employee_lib)
                    target_link_libraries(${test_name} PRIVATE employee_lib common_lib GTest::gtest_main)
                else()
                    target_link_libraries(${test_name} PRIVATE common_lib GTest::gtest_main)
                endif()
            elseif(test_name MATCHES ".*[Cc]lient.*" OR test_name MATCHES ".*[Kk]lijent.*")
                if(TARGET client_lib)
                    target_link_libraries(${test_name} PRIVATE client_lib common_lib GTest::gtest_main)
                else()
                    target_link_libraries(${test_name} PRIVATE common_lib GTest::gtest_main)
                endif()
            else()
                target_link_libraries(${test_name} PRIVATE common_lib GTest::gtest_main)
            endif()
            
            add_test(NAME ${test_name} COMMAND ${test_name})
        endforeach()
    else()
        message(WARNING "No test source files found in tests/ directory")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Only install if executables were created
if(TARGET KlijentApp AND TARGET ZaposleniApp)
    install(TARGETS KlijentApp ZaposleniApp RUNTIME DESTINATION bin)
endif()