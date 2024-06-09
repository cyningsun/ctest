cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(folly-fetchcontent LANGUAGES CXX)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

include(../../cmake/utils.cmake)
include(boost.cmake)
include(double-conversion.cmake)
include(fmt.cmake)
include(gflags.cmake)
include(glog.cmake)
include(libevent.cmake)
include(sodium.cmake)