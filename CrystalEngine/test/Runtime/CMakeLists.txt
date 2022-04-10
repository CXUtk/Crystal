cmake_minimum_required (VERSION 3.11)

# set libraries
set(GTEST_LIB gtest gtest_main)
set(CRYSTAL_ENGINE_RUNTIME_LIB "CrystalEngine_Runtime")

# set compile flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

link_libraries(${CRYSTAL_ENGINE_RUNTIME_LIB} ${GTEST_LIB})

add_executable(CrystalUT_Utils "Utils_Test.cpp")
add_executable(CrystalUT_FileSystem "FileSystem_Test.cpp")
# target_link_libraries(CrystalUT_Utils PUBLIC ${CRYSTAL_ENGINE_RUNTIME_LIB} ${GTEST_LIB})
# target_link_libraries(CrystalUT_FileSystem PUBLIC ${CRYSTAL_ENGINE_RUNTIME_LIB} ${GTEST_LIB})

add_test(NAME CrystalUT_Utils COMMAND CrystalUT_Utils)
add_test(NAME CrystalUT_FileSystem COMMAND CrystalUT_FileSystem)