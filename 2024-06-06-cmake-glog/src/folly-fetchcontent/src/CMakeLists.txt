cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(folly-fetchcontent LANGUAGES CXX)


include(cmake/folly.cmake)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

find_package(folly REQUIRED)

add_executable(folly-fetchcontent main.cpp)

target_link_libraries(folly-fetchcontent 
    PUBLIC
    folly::folly
)