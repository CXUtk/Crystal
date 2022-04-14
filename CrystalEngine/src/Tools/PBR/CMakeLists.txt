cmake_minimum_required (VERSION 3.14)

add_executable ("IrradianceMapGenerator"
"IrradianceMapGenerator.h"
"IrradianceMapGenerator.cpp")

set_target_properties("IrradianceMapGenerator"
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tools/PBR"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tools/PBR")

set_target_properties("IrradianceMapGenerator" PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
CXX_EXTENSIONS OFF)

target_link_libraries("IrradianceMapGenerator" PRIVATE "CrystalEngine_Runtime")


add_executable ("LUTGenerator" "LUTGenerator.cpp")

set_target_properties("LUTGenerator"
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tools/PBR"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tools/PBR")

set_target_properties("LUTGenerator" PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
CXX_EXTENSIONS OFF)

target_link_libraries("LUTGenerator" PRIVATE "CrystalEngine_Runtime")
