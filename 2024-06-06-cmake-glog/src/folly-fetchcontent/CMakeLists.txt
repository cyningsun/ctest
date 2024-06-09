cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(folly-fetchcontent LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(../cmake/utils.cmake)
include(cmake/boost.cmake)
include(cmake/double-conversion.cmake)

include(cmake/folly.cmake)


find_package(folly REQUIRED)

add_executable(folly-fetchcontent main.cpp)

target_link_libraries(folly-fetchcontent 
    PUBLIC
    folly::folly
)