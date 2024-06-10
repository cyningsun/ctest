cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(boost-externalproj LANGUAGES CXX)

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


include(CMakePrintHelpers)
cmake_print_variables(folly_external_ROOT)
cmake_print_variables(folly_external_INCLUDEDIR)
cmake_print_variables(folly_external_LIBRARYDIR)

cmake_print_variables(fmt_external_ROOT)
cmake_print_variables(fmt_external_INCLUDEDIR)
cmake_print_variables(fmt_external_LIBRARYDIR)
SSE@10-129-115-107> git:(feature_cmake_glog) ✗  cat external/folly/CMakeLists.txt

set(folly_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -momit-leaf-frame-pointer -DHAVE_CXX11_ATOMIC")
set(folly_CXX_M_FLAGS "${CXX_M_FLAGS} -march=native -momit-leaf-frame-pointer")

find_package(folly QUIET)
if (folly_FOUND)
    message(STATUS "Found folly version ${folly_MAJOR_VERSION}.${folly_MINOR_VERSION}.${folly_SUBMINOR_VERSION}")
    add_library(folly_external INTERFACE)
else()
    include(ExternalProject)
    ExternalProject_Add(folly_external
        URL https://github.com/facebook/folly/archive/refs/tags/v2022.06.20.00.zip
        DOWNLOAD_NO_PROGRESS
          1
        UPDATE_COMMAND
          <SOURCE_DIR>/build.sh --scratch-path ${STAGED_INSTALL_PREFIX}/folly --no-tests
        CONFIGURE_COMMAND
          ""
        BUILD_COMMAND
	        <SOURCE_DIR>/build.sh --scratch-path ${STAGED_INSTALL_PREFIX}/folly --no-tests
        CMAKE_ARGS
          -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
          -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
          -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
          -DBUILD_SHARED_LIBS=OFF
          -DBOOST_LINK_STATIC=ON
          -DBUILD_TESTS=OFF
          -DCMAKE_BUILD_TYPE=RelWithDebInfo
        CMAKE_CACHE_ARGS
          -DCMAKE_CXX_FLAGS:STRING=${folly_CMAKE_CXX_FLAGS}
          -DCXX_M_FLAGS:STRING=${folly_folly_CXX_M_FLAGS}
       BUILD_IN_SOURCE
	        1
       BUILD_ALWAYS
          1
        LOG_BUILD
          1
        BUILD_IN_SOURCE
          1
        INSTALL_COMMAND
          ""
        LOG_INSTALL
          1
    )

  set(
    folly_external_ROOT ${STAGED_INSTALL_PREFIX}/folly/installed/folly
    CACHE PATH "Path to internally built folly installation root"
    FORCE
  )
  set(
    folly_external_INCLUDEDIR ${folly_external_ROOT}/include
    CACHE PATH "Path to internally built folly include directories"
    FORCE
  )
  set(
    folly_external_LIBRARYDIR ${folly_external_ROOT}/lib
    CACHE PATH "Path to internally built folly library directories"
    FORCE
  )

  set(
    fmt_external_ROOT ${STAGED_INSTALL_PREFIX}/folly/installed/fmt-bD_a1aH7EZB7vioXuPEFXDRQyf06Gfz8Axp4Q_vbW60
    CACHE PATH "Path to internally built fmt installation root"
    FORCE
  )
  set(
    fmt_external_INCLUDEDIR ${fmt_external_ROOT}/include
    CACHE PATH "Path to internally built fmt include directories"
    FORCE
  )
  set(
    fmt_external_LIBRARYDIR ${fmt_external_ROOT}/lib
    CACHE PATH "Path to internally built fmt library directories"
    FORCE
  )

  set(
    boost_external_ROOT ${STAGED_INSTALL_PREFIX}/folly/installed/boost-OgoNAcUfk905p7ilmG3Za9baApIB3gskpGAV_QxkJE0
    CACHE PATH "Path to internally built boost installation root"
    FORCE
  )
  set(
    boost_external_INCLUDEDIR ${boost_external_ROOT}/include
    CACHE PATH "Path to internally built boost include directories"
    FORCE
  )
  set(
    boost_external_LIBRARYDIR ${boost_external_ROOT}/lib
    CACHE PATH "Path to internally built boost library directories"
    FORCE
  )

  set(
    gflags_external_ROOT ${STAGED_INSTALL_PREFIX}/folly/installed/gflags-kizJ8w9CrsofsLEi7uTEv88iWsnKXRLDg2P-CD0RUvw
    CACHE PATH "Path to internally built gflags installation root"
    FORCE
  )
  set(
    gflags_external_INCLUDEDIR ${gflags_external_ROOT}/include
    CACHE PATH "Path to internally built gflags include directories"
    FORCE
  )
  set(
    gflags_external_LIBRARYDIR ${gflags_external_ROOT}/lib
    CACHE PATH "Path to internally built gflags library directories"
    FORCE
  )

endif()