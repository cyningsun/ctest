

cmake_minimum_required (VERSION 3.16)
project (glog-fetchcontent VERSION 1.0)

include(../cmake/utils.cmake)

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

add_executable (glog-fetchcontent main.cpp)
target_link_libraries (glog-fetchcontent glog::glog)