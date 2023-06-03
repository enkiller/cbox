cmake_minimum_required(VERSION 3.10)

project(Cbox)

# define options
option(ASAN "Enable AddressSanitizer" OFF)
option(COVERAGE "Enable code coverage" OFF)
option(STRICT_ERROR "Treats all compiler warnings as errors." OFF)
option(CI_BUILD "Configure GitHub CI compilation." OFF)

# basic configuration
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (NOT CI_BUILD)
  configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()

  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                  EXCLUDE_FROM_ALL)

  # The gtest/gtest_main targets carry header search path
  # dependencies automatically when using CMake 2.8.11 or
  # later. Otherwise we have to add them here ourselves.
  if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
  endif()
endif()

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

# build target
add_executable (Cbox-test)

# add include path
include_directories(include)

# add source file
add_subdirectory(src)
add_subdirectory(test)

if (CI_BUILD)
# Now simply link against gtest_main or gtest_main as needed. 
TARGET_LINK_LIBRARIES(Cbox-test gtest pthread)
else ()
TARGET_LINK_LIBRARIES(Cbox-test gtest_main)
endif ()

enable_testing()