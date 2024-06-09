include_guard()

FetchContent_DeclareGitHubWithMirror(libevent
  libevent/libevent release-2.1.12-stable
  MD5=041edf4f20251f429d1674759ab6882c
)

set(libevent_disable_ssl ON)
if(ENABLE_OPENSSL)
  set(libevent_disable_ssl OFF)
endif()

FetchContent_MakeAvailableWithArgs(libevent
  EVENT__DISABLE_TESTS=ON
  EVENT__DISABLE_REGRESS=ON
  EVENT__DISABLE_SAMPLES=ON
  EVENT__DISABLE_OPENSSL=${libevent_disable_ssl}
  EVENT__DISABLE_MBEDTLS=ON
  EVENT__LIBRARY_TYPE=STATIC
  EVENT__DISABLE_BENCHMARK=ON
  EVENT__DISABLE_DEBUG_MODE=ON
)
