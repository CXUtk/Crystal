cmake_minimum_required (VERSION 3.11)

file(GLOB_RECURSE CrystalEngine_UnitTests_Utils "Core/Utils_test.cpp")
add_executable(CrystalUT_Utils ${CrystalEngine_UnitTests_Utils})
target_include_directories(CrystalUT_Utils SYSTEM PUBLIC ../src)
target_link_libraries(CrystalUT_Utils PRIVATE CrystalEngine)
add_dependencies(CrystalUT_Utils CrystalEngine)

add_test(NAME CrystalUT_Utils COMMAND CrystalUT_Utils)
set_tests_properties(CrystalUT_Utils PROPERTIES
    ENVIRONMENT "TEST_FILE=CrystalUT_Utils.cpp;TEST_LINE=1"
)