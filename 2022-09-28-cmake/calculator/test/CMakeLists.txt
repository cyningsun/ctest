option(ENABLE_UNIT_TESTS "Enable uinit tests" on)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

include(googletest.cmake)

if(ENABLE_UNIT_TESTS)
    fetch_googletest(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/googletest
    )

    add_executable(unit_tests "")

    target_sources(
        unit_tests
        PRIVATE
        calculator.cpp
        main.cpp
    )

    target_link_libraries(
        unit_tests
        PRIVATE
        calc
        gtest_main
    )

    add_test(unit ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)
endif()
