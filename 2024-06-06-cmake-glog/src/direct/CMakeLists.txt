

cmake_minimum_required (VERSION 3.16)
project (glogdirectproj VERSION 1.0)

include(FetchContent)

macro(parse_var arg key value)
  string(REGEX REPLACE "^(.+)=(.+)$" "\\1;\\2" REGEX_RESULT ${arg})
  list(GET REGEX_RESULT 0 ${key})
  list(GET REGEX_RESULT 1 ${value})
endmacro()

function(FetchContent_MakeAvailableWithArgs dep)
  if(NOT ${dep}_POPULATED)
    message("Fetching ${dep}...")
    FetchContent_Populate(${dep})

    foreach(arg IN LISTS ARGN)
      parse_var(${arg} key value)
      set(${key}_OLD ${${key}})
      set(${key} ${value} CACHE INTERNAL "")
    endforeach()

    add_subdirectory(${${dep}_SOURCE_DIR} ${${dep}_BINARY_DIR} EXCLUDE_FROM_ALL)

    foreach(arg IN LISTS ARGN)
      parse_var(${arg} key value)
      set(${key} ${${key}_OLD} CACHE INTERNAL "")
    endforeach()
  endif()
endfunction()

function(FetchContent_DeclareWithMirror dep url hash)
  FetchContent_Declare(${dep}
    URL ${DEPS_FETCH_PROXY}${url}
    URL_HASH ${hash}
  )
endfunction()

function(FetchContent_DeclareGitHubWithMirror dep repo tag hash)
  FetchContent_DeclareWithMirror(${dep}
    https://github.com/${repo}/archive/${tag}.zip
    ${hash}
  )
endfunction()


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

add_executable (glog-direct main.cpp)
target_link_libraries (glog-direct glog::glog)