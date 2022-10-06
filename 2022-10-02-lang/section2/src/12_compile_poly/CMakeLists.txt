add_library(gcd "gcd.cpp")

include(FetchContent)
FetchContent_Declare(
  cln
  GIT_REPOSITORY git://www.ginac.de/cln.git
  GIT_TAG       master
)

FetchContent_MakeAvailable(cln)

target_link_libraries(
    gcd 
    PRIATE
    cln
)