include_guard()


if(NOT TARGET glog)

    FetchContent_Declare(
    glog
    URL https://github.com/google/glog/archive/v0.5.0.tar.gz
    )


    FetchContent_MakeAvailableWithArgs(glog)

endif()