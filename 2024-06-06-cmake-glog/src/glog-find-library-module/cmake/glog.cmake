include_guard()


if(NOT TARGET glog::glog)
  FetchContent_DeclareGitHubWithMirror(glog
    google/glog v0.6.0
    MD5=1b246d4d0e8a011d33e0813b256198ef
  )

  FetchContent_MakeAvailableWithArgs(glog
    WITH_GFLAGS=OFF
    WITH_GTEST=OFF
    BUILD_SHARED_LIBS=OFF
    WITH_UNWIND=OFF
  )
endif()

