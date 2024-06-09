include_guard()

FetchContent_DeclareGitHubWithMirror(double-conversion
  google/double-conversion v3.1.4
  MD5=852c3430c9a2f742e887729e7bd19194
)

FetchContent_MakeAvailable(double-conversion)

SET(DOUBLE_CONVERSION_LIBRARY ${double-conversion_SOURCE_DIR}/double-conversion)
SET(DOUBLE_CONVERSION_INCLUDE_DIR ${double-conversion_SOURCE_DIR}/include)