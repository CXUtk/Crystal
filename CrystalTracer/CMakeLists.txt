# CMakeList.txt: Crystal 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.14)

file(GLOB_RECURSE CrystalTracerSources CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE CrystalTracerHeaders CONFIGURE_DEPENDS "src/*.h")


# 将源代码添加到此项目的可执行文件。
add_executable (CrystalTracer ${CrystalTracerSources} ${CrystalTracerHeaders})

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
target_compile_features(CrystalEngine PRIVATE cxx_std_17)
endif(MSVC)

target_include_directories(CrystalTracer SYSTEM PUBLIC "src")
target_include_directories(CrystalTracer SYSTEM PRIVATE "..")

target_link_libraries(CrystalTracer PRIVATE CrystalEngine)
add_dependencies(CrystalTracer CrystalEngine)

execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/resources" "${PROJECT_BINARY_DIR}/CrystalTracer")