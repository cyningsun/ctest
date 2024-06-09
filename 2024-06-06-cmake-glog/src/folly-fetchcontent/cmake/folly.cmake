include_guard()


FetchContent_Declare(
  folly
  GIT_REPOSITORY https://github.com/facebook/folly.git
  GIT_TAG main
)


FetchContent_MakeAvailableWithArgs(folly)