
cmake_minimum_required (VERSION 3.14)

project ("Crystal" VERSION 0.0.1)

# ---- Include guards ----
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

set(CRYSTAL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_INSTALL_PREFIX "${CRYSTAL_ROOT_DIR}/install")
set(BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/")

message(${CMAKE_INSTALL_PREFIX})

add_subdirectory ("CrystalTracer")
add_subdirectory ("CrystalEngine")
