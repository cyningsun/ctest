cmake_minimum_required(VERSION 3.0)

project(ascii_tetris LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS off)
set(CMAKE_C_STANDARD_REQUIRED on)

add_executable(ascii_tetris)

target_include_directories(ascii_tetris
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(ascii_tetris
    PRIVATE
    main.c
    tetris.c
)