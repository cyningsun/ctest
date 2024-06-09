include_guard()

set(BOOST_INCLUDE_LIBRARIES context filesystem program_options regex system thread)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_Declare(
  Boost
  URL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz
)
FetchContent_MakeAvailable(Boost)