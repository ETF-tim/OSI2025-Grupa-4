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



if(COMMON_SRC)
    add_library(common_lib STATIC ${COMMON_SRC})
    target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
else()
    message(WARNING "No source files found in src/common/. Creating interface library.")
    add_library(common_lib INTERFACE)
    target_include_directories(common_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

if(EMPLOYEE_MODULE_SRC)
    add_library(employee_lib STATIC ${EMPLOYEE_MODULE_SRC})
    target_include_directories(employee_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(employee_lib PUBLIC common_lib)
else()
    message(WARNING "No source files found in src/employee/")
endif()

if(CLIENT_MODULE_SRC)
    add_library(client_lib STATIC ${CLIENT_MODULE_SRC})
    target_include_directories(client_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(client_lib PUBLIC common_lib)
else()
    message(WARNING "No source files found in src/client/")
endif()

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
            
            set(TEST_LIBS GTest::gtest_main common_lib)
            
            if(TARGET employee_lib AND NOT test_name MATCHES ".*[Cc]lient.*|.*[Kk]lijent.*")
                list(APPEND TEST_LIBS employee_lib)
            endif()
            
            if(TARGET client_lib AND test_name MATCHES ".*[Cc]lient.*|.*[Kk]lijent.*")
                list(APPEND TEST_LIBS client_lib)
            endif()
            
            target_link_libraries(${test_name} PRIVATE ${TEST_LIBS})
            add_test(NAME ${test_name} COMMAND ${test_name})
        endforeach()
    else()
        message(WARNING "No test source files found in tests/ directory")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(TARGET KlijentApp AND TARGET ZaposleniApp)
    install(TARGETS KlijentApp ZaposleniApp RUNTIME DESTINATION bin)
endif()