
cmake_minimum_required (VERSION 3.16)
project (gflags-fetchcontent VERSION 1.0)

include(../cmake/utils.cmake)

if(NOT TARGET gflags::gflags)

  FetchContent_DeclareGitHubWithMirror(gflags
    gflags/gflags v2.2.2
    MD5=ff856ff64757f1381f7da260f79ba79b
  )

  FetchContent_MakeAvailableWithArgs(gflags
    WITH_GFLAGS=OFF
    WITH_GTEST=OFF
    BUILD_SHARED_LIBS=OFF
    WITH_UNWIND=OFF
  )

endif()

add_executable(gflags-fetchcontent main.cpp)
if (GFLAGS_USE_TARGET_NAMESPACE)
  if (TARGET gflags::gflags)
    target_link_libraries(gflags-fetchcontent gflags::gflags)
  else ()
    message(FATAL_ERROR "gflags::gflags target not defined")
  endif ()
else ()
  if (TARGET gflags)
    target_link_libraries(gflags-fetchcontent gflags)
  else ()
    message(FATAL_ERROR "gflags target not defined")
  endif ()
endif ()