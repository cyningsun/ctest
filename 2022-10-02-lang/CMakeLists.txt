cmake_minimum_required(VERSION 3.5...3.18 FATAL_ERROR)

project(Lang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS on)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_subdirectory(section1/src/02_smart_ptr)
add_subdirectory(section1/src/03_move)
add_subdirectory(section2/src/12_compile_poly)
add_subdirectory(section2/src/13_tpl_meta_prog)
add_subdirectory(section2/src/19_future)
add_subdirectory(section2/src/30_corutines)