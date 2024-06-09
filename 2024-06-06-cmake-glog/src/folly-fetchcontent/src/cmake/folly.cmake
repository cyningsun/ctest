include_guard()

include(FetchContent)

FetchContent_Declare(
  folly
  GIT_REPOSITORY https://github.com/facebook/folly.git
  GIT_TAG v2022.06.20.00
)

FetchContent_MakeAvailable(folly)