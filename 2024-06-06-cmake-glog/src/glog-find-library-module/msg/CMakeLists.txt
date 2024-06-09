
find_package(glog REQUIRED)

add_library(msg
  STATIC
    msg.hpp
    msg.cpp
  )

target_link_libraries (msg glog)