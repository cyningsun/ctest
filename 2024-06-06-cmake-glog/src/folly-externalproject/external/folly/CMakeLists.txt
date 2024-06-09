find_package(folly QUIET)
if (folly_FOUND)
    message(STATUS "Found folly version ${folly_MAJOR_VERSION}.${folly_MINOR_VERSION}.${folly_SUBMINOR_VERSION}")
    add_library(folly_external INTERFACE)
else()
    include(ExternalProject)
    ExternalProject_Add(folly_external
        URL https://github.com/facebook/folly/archive/refs/tags/v2022.06.20.00.zip
        DOWNLOAD_NO_PROGRESS
        1
        UPDATE_COMMAND
        ""
        CONFIGURE_COMMAND
        ""
        BUILD_COMMAND
            cd <SOURCE_DIR> && ./build/fbcode_builder/getdeps.py build 
                --scratch-path ${STAGED_INSTALL_PREFIX}/folly  
                --no-tests
        LOG_BUILD
        1
        BUILD_IN_SOURCE
        1
        INSTALL_COMMAND
        ""
        LOG_INSTALL
        1
    )
endif()