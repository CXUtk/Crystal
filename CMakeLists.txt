# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("CrystalTracer")

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

# 包含子项目。
add_subdirectory ("CrystalTracer")
add_subdirectory ("CrystalEngine")
