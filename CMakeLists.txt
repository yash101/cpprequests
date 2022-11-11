cmake_minimum_required(VERSION 3.12)
project(cpprequests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CPPREQUESTS_BASE_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(CPPREQUESTS_BASE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CPPREQUESTS_INCLUDE_DIR ${CPPREQUESTS_BASE_SRC_DIR}/include)

option(BUILD_LIBCURL "Build libcURL from source" ON)

IF (BUILD_LIBCURL)
  find_package(CURL)
ENDIF()

IF (NOT CURL_FOUND OR BUILD_LIBCURL)
  include(cmake/build_curl.cmake)
ELSE ()
ENDIF ()

include(cmake/nlohmann_json.cmake)

add_subdirectory(src)