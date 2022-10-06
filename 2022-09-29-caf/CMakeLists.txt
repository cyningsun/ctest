cmake_minimum_required(VERSION 3.0.0)
project(cafdemo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include(FetchContent)
FetchContent_Declare(caf
                     GIT_REPOSITORY https://github.com/actor-framework/actor-framework.git
                     GIT_TAG        0.18.6)
FetchContent_MakeAvailable(caf)


add_subdirectory(01-helloworld/src)
add_subdirectory(02-message-passing/src)

