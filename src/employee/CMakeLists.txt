file(GLOB_RECURSE EMPLOYEE_SOURCES 
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if(EMPLOYEE_SOURCES)
    add_library(employee_lib STATIC ${EMPLOYEE_SOURCES})
    
    target_include_directories(employee_lib 
        PUBLIC 
            ${PROJECT_SOURCE_DIR}/include
    )
    
    target_link_libraries(employee_lib 
        PUBLIC 
            common_lib
    )
else()
    message(WARNING "No .cpp files found in src/employee/")
    add_library(employee_lib INTERFACE)
    target_link_libraries(employee_lib INTERFACE common_lib)
endif()