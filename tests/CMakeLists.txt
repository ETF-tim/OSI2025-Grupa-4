message(STATUS "=== Entering tests/CMakeLists.txt ===")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR:  ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Pronađi test fajlove - probaj oba načina
file(GLOB TEST_FILES_REL "*.cpp")
file(GLOB TEST_FILES_ABS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB TEST_FILES_PROJ "${PROJECT_SOURCE_DIR}/tests/*.cpp")

message(STATUS "TEST_FILES_REL: ${TEST_FILES_REL}")
message(STATUS "TEST_FILES_ABS: ${TEST_FILES_ABS}")
message(STATUS "TEST_FILES_PROJ: ${TEST_FILES_PROJ}")

# Koristi prvu verziju koja ima sadržaj
if(TEST_FILES_ABS)
    set(TEST_FILES ${TEST_FILES_ABS})
elseif(TEST_FILES_PROJ)
    set(TEST_FILES ${TEST_FILES_PROJ})
elseif(TEST_FILES_REL)
    set(TEST_FILES ${TEST_FILES_REL})
else()
    message(FATAL_ERROR "No test files found!")
endif()

message(STATUS "Using TEST_FILES: ${TEST_FILES}")

# Funkcija za dodavanje testova
function(add_unit_test test_name test_source)
    message(STATUS "Creating test:  ${test_name}")
    add_executable(${test_name} ${test_source})
    
    set(LIBS common_lib GTest::gtest_main)
    
    if(test_name MATCHES ".*[Cc]lient.*")
        list(APPEND LIBS client_lib)
    elseif(test_name MATCHES ".*[Ee]mployee.*|.*[Zz]aposleni.*")
        list(APPEND LIBS employee_lib)
    endif()
    
    target_link_libraries(${test_name} PRIVATE ${LIBS})
    add_test(NAME ${test_name} COMMAND ${test_name})
    message(STATUS "  ✓ Registered test: ${test_name}")
endfunction()

# Kreiraj testove
foreach(test_file ${TEST_FILES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_unit_test(${test_name} ${test_file})
endforeach()

message(STATUS "=== Finished tests/CMakeLists.txt ===")