cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(folly-externalproj LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)

set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
message(STATUS "${PROJECT_NAME} staged install: ${STAGED_INSTALL_PREFIX}")

add_subdirectory(external)

include(ExternalProject)
ExternalProject_Add(${PROJECT_NAME}
  DEPENDS
    folly_external
  SOURCE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/src
  CMAKE_ARGS
    -Dfmt_DIR=${fmt_external_ROOT}/lib/cmake/fmt 
    -Dfolly_DIR=${folly_external_ROOT}/lib/cmake/folly 
    -DBOOST_ROOT=${boost_external_ROOT}
    #-DCMAKE_PREFIX_PATH=${fmt_external_ROOT}/lib/cmake/fmt
    -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
    -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
    -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
  CMAKE_CACHE_ARGS
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_INCLUDE_PATH:PATH=${folly_external_INCLUDEDIR}:${gflags_external_INCLUDEDIR}
    -DCMAKE_LIBRARY_PATH:PATH=${folly_external_LIBRARYDIR}:${gflags_external_LIBRARYDIR}
  BUILD_ALWAYS
    1
  INSTALL_COMMAND
    ""
)