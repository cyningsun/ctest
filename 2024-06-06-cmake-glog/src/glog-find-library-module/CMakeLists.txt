

cmake_minimum_required (VERSION 3.16)
project (glog-fetchcontent-module VERSION 1.0)


include(../cmake/utils.cmake)
include(cmake/glog.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

add_subdirectory(msg)

add_executable (glog-fetchcontent-module main.cpp)
target_link_libraries (glog-fetchcontent-module msg)