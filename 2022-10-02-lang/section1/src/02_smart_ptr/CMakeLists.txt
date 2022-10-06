add_executable(smart_test "")

target_include_directories(
    smart_test
    PRIVATE
    smart_ptr.hpp
)

target_sources(
    smart_test
    PRIVATE
    main.cpp
)