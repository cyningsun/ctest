add_library(calc SHARED "")

target_sources(
    calc
    PRIVATE
    add.f90
    subtract.f90
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/calculator.h
)

target_include_directories(
    calc
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${PROJECT_BINARY_DIR}/generated
  )

add_executable(calculator "")
target_sources(
    calculator
    PRIVATE
    main.cpp
)

target_link_libraries(
    calculator
    PRIVATE
    calc
)

install(
    TARGETS calculator
    RUNTIME DESTINATION ${CMAKE_INSTALLBINDIR}
)

install(
    TARGETS calc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)