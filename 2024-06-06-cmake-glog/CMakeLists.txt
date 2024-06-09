

cmake_minimum_required (VERSION 3.16)
project (glogproj VERSION 1.0)


add_subdirectory(src/glog-fetchcontent)
add_subdirectory(src/gflags-fetchcontent)
add_subdirectory(src/glog-fetchcontent-module)
add_subdirectory(src/boost-externalproject)
add_subdirectory(src/boost-fetchcontent)