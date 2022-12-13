cmake_minimum_required(VERSION 3.12)
project(cpprequests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CPPREQUESTS_BASE_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(CPPREQUESTS_BASE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CPPREQUESTS_INCLUDE_DIR ${CPPREQUESTS_BASE_SRC_DIR}/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake"
    TLS_VERIFY ON
  )
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

# Set up dependencies
conan_cmake_configure(
  REQUIRES
    nlohmann_json/3.11.2
    libcurl/7.86.0
  GENERATORS
    cmake_find_package
)

conan_cmake_autodetect(settings)

conan_cmake_install(
  PATH_OR_REFERENCE .
  BUILD missing
  REMOTE conancenter
  SETTINGS ${settings}
)

find_package(nlohmann_json)

add_subdirectory(src)
