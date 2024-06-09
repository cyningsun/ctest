include_guard()


FetchContent_DeclareGitHubWithMirror(gflags
    gflags/gflags v2.2.2
    MD5=ff856ff64757f1381f7da260f79ba79b
)

FetchContent_MakeAvailableWithArgs(gflags
  BUILD_SHARED_LIBS=OFF
  WITH_UNWIND=OFF
)