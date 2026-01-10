file(GLOB_RECURSE CLIENT_SOURCES 
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if(CLIENT_SOURCES)
    add_library(client_lib STATIC ${CLIENT_SOURCES})
    
    target_include_directories(client_lib 
        PUBLIC 
            ${PROJECT_SOURCE_DIR}/include
    )
    
    target_link_libraries(client_lib 
        PUBLIC 
            common_lib
    )
else()
    message(WARNING "No . cpp files found in src/client/")
    add_library(client_lib INTERFACE)
    target_link_libraries(client_lib INTERFACE common_lib)
endif()