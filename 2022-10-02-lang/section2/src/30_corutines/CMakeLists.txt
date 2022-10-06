add_compile_options(-stdlib=libc++ -fcoroutines-ts)

add_executable(fibonacci_coroutines_ts fibonacci_coroutines_ts.cpp)