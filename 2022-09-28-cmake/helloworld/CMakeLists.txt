cmake_minimum_required(VERSION 3.0)

project(helloworld LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(hello "")

target_sources(hello
    PRIVATE
    hello.cxx
)