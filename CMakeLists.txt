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

# Add subdirectories
add_subdirectory(src/common)
add_subdirectory(src/client)
add_subdirectory(src/employee)

# Build executables
add_executable(KlijentApp src/mainClient.cpp)
target_link_libraries(KlijentApp PRIVATE client_lib)

add_executable(ZaposleniApp src/mainEmployee.cpp)
target_link_libraries(ZaposleniApp PRIVATE employee_lib)

# Tests
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS KlijentApp ZaposleniApp 
        RUNTIME DESTINATION bin)