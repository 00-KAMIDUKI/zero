cmake_minimum_required(VERSION 3.29)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(FATAL_ERROR "In-source builds are not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

project(zero)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(ZERO_IS_MAIN_PROJECT ON)
else()
  set(ZERO_IS_MAIN_PROJECT OFF)
endif()

option(ZERO_BUILD_TESTS "Build tests" ${ZERO_IS_MAIN_PROJECT})

if (ZERO_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()


