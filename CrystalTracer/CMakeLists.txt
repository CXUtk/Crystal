# CMakeList.txt: Crystal 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE CrystalTracerSources "src/*.cpp")
file(GLOB_RECURSE CrystalTracerHeaders "src/*.h")


# 将源代码添加到此项目的可执行文件。
add_executable (CrystalTracer ${CrystalTracerSources} ${CrystalTracerHeaders})

set_target_properties(CrystalTracer PROPERTIES
            CXX_STANDARD 20
            CXX_EXTENSIONS OFF
            )

target_include_directories(CrystalTracer SYSTEM PUBLIC "src")
target_include_directories(CrystalTracer SYSTEM PRIVATE "..")

target_link_libraries(CrystalTracer PRIVATE CrystalEngine)
add_dependencies(CrystalTracer CrystalEngine)

execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/resources" "${PROJECT_BINARY_DIR}/CrystalTracer")