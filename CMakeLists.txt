cmake_minimum_required(VERSION 3.10)

project(Cbox)

# define options
option(ASAN "Enable AddressSanitizer" OFF)
option(COVERAGE "Enable code coverage" OFF)
option(STRICT_ERROR "Treats all compiler warnings as errors." OFF)

# basic configuration
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (STRICT_ERROR)
  if (MSVC)
    add_compile_options("/WX")
  else ()
    add_compile_options("-Werror")
  endif ()
endif ()

if (ASAN)
  message(STATUS "Enable AddressSanitizer.")
  if (MSVC)
    add_compile_options("/fsanitize=address")
  else ()
    add_compile_options("-fno-omit-frame-pointer" "-fsanitize=address" "-fsanitize=undefined")
    add_link_options("-fno-omit-frame-pointer" "-fsanitize=address" "-fsanitize=undefined")
  endif ()
endif ()

if (COVERAGE)
  message(STATUS "Enable code coverage.")
  if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_compile_options("-fprofile-instr-generate" "-fcoverage-mapping")
    add_link_options("-fprofile-instr-generate" "-fcoverage-mapping")
  elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    add_compile_options("--coverage")
    add_link_options("--coverage")
  else ()
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} does not support coverage analysis.")
  endif ()
endif ()

# compiler options
if (MSVC)
  add_compile_options("/MP")
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_compile_options("-Wall" "-Wextra")
endif ()

# add include path
include_directories(include)

# add source file
file(GLOB SOURCES ./src/container/hashmap/*.c ./src/libc/*c ./test/*cpp)

# build
add_executable (Cbox-test ${SOURCES})
TARGET_LINK_LIBRARIES(Cbox-test gtest pthread)
