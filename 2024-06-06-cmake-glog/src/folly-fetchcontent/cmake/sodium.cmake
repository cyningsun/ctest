include_guard()

FetchContent_Declare(
    sodium
    URL https://github.com/jedisct1/libsodium/releases/download/1.0.17/libsodium-1.0.17.tar.gz
)

if(NOT sodium_POPULATED)
    FetchContent_Populate(sodium)
    
    execute_process(COMMAND ${sodium_SOURCE_DIR}/configure
        WORKING_DIRECTORY ${sodium_BINARY_DIR}
    )

    add_custom_target(make_sodium 
        COMMAND ${MAKE_COMMAND}
        WORKING_DIRECTORY ${sodium_BINARY_DIR}
        #BYPRODUCTS ${sodium_BINARY_DIR}/src/libsodium/.libs/libsodium.a
    )
endif()

add_library(sodium INTERFACE)
target_include_directories(sodium INTERFACE $<BUILD_INTERFACE:${sodium_BINARY_DIR}/src/libsodium/include>)
target_link_libraries(sodium INTERFACE $<BUILD_INTERFACE:${sodium_BINARY_DIR}/src/libsodium/.libs/libsodium.a>)
add_dependencies(sodium make_sodium)