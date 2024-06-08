

cmake_minimum_required (VERSION 3.16)
project (glogindirectproj VERSION 1.0)


include(cmake/glog.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

add_subdirectory(msg)

add_executable (glog-indirect main.cpp)
target_link_libraries (glog-indirect msg)