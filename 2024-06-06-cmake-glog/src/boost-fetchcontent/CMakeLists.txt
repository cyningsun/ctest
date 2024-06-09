cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(boost-fetchcontent LANGUAGES CXX)


include(../cmake/utils.cmake)
include(cmake/boost.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

find_package(Boost 1.61 REQUIRED COMPONENTS filesystem)

add_executable(boost-fetchcontent main.cpp)

target_link_libraries(boost-fetchcontent
  PUBLIC
    Boost::filesystem
)
