cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(boost-externalproj_core LANGUAGES CXX)

find_package(folly REQUIRED)

add_executable(boost-externalproj_core main.cpp)

target_link_libraries(boost-externalproj_core
  PUBLIC
  folly::folly
)
