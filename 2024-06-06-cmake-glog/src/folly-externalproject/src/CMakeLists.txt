cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(folly-externalproj_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(folly REQUIRED)

# Fix linking issue for Debug configuration on ubuntu 22.04 
find_package(gflags CONFIG REQUIRED)
set_target_properties(gflags_shared PROPERTIES
    MAP_IMPORTED_CONFIG_DEBUG Release
)

add_executable(folly-externalproj_core main.cpp)

target_link_libraries(folly-externalproj_core
  PUBLIC
  Folly::folly
)