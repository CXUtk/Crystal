# CMakeList.txt: Crystal 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(CrystalEngine)

option(CRYSTAL_USE_OPENGL "Use OpenGL and GLFW for graphics interface" OFF)
option(CRYSTAL_USE_DX11 "Use DX11 and Win32 for graphics interface" ON)
option(CRYSTAL_USE_DEBUG "Enable debug infomation and outputs" ON)

if (UNIX)
    add_compile_definitions(CRYSTAL_USE_UNIX)
elseif(WIN32)
    add_compile_definitions(CRYSTAL_USE_WIN32)
else()
    message("Unsupported operating system")
endif()

if (CRYSTAL_USE_OPENGL)
    add_compile_definitions(CRYSTAL_USE_OPENGL)
endif()
if (CRYSTAL_USE_DX11)
    add_compile_definitions(CRYSTAL_USE_DX11)
endif()
if (CRYSTAL_USE_DEBUG)
    add_compile_definitions(CRYSTAL_USE_DEBUG)
endif()

enable_testing()
add_subdirectory ("external/gtest")
include_directories("external/gtest/googletest/include")
add_subdirectory ("src")
add_subdirectory ("test")
# TODO: 如有需要，请添加测试并安装目标。

