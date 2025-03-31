cmake_minimum_required(VERSION 3.10)
project(heartbeat-sched)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread")

# 寻找必要的库
find_package(Threads REQUIRED)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/src)

# 添加可执行文件
add_executable(heartbeat_demo 
    src/main.cpp
    src/stats.cpp
    src/heartbeat.cpp)

# 链接线程库
target_link_libraries(heartbeat_demo PRIVATE Threads::Threads rt)