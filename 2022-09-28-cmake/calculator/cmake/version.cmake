function(generate_version_h)
    set(GIT_HASH "unknown")

    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} --no-pager show -s --pretty=format:%h -n 1
            OUTPUT_VARIABLE GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            ERROR_QUIET
        )
    endif()
    
    configure_file(
        cmake/version.h.in
        generated/version.h
        @ONLY
    )
endfunction()
