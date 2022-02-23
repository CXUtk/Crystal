cmake_minimum_required (VERSION 3.11)

set(GTEST_LIB gtest gtest_main)

add_executable(CrystalUT_Utils "Core/Utils_Test.cpp")
target_link_libraries(CrystalUT_Utils PUBLIC CrystalEngine ${GTEST_LIB})
add_dependencies(CrystalUT_Utils CrystalEngine)
set_target_properties(CrystalUT_Utils PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF)

add_executable(CrystalUT_FileSystem "Core/FileSystem_Test.cpp")
target_link_libraries(CrystalUT_FileSystem PUBLIC CrystalEngine ${GTEST_LIB})
add_dependencies(CrystalUT_FileSystem CrystalEngine)
set_target_properties(CrystalUT_FileSystem PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF)

add_test(NAME CrystalUT_Utils COMMAND CrystalUT_Utils)
add_test(NAME CrystalUT_FileSystem COMMAND CrystalUT_FileSystem)