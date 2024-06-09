include_guard()


if(NOT TARGET gflags::gflags)

    FetchContent_Declare(
    gflags
    URL https://github.com/gflags/gflags/archive/v2.2.2.tar.gz
    )


    FetchContent_MakeAvailableWithArgs(gflags
    BUILD_SHARED_LIBS=OFF
    WITH_UNWIND=OFF
    )

endif()