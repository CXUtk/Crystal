# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("CrystalTracer")

if (UNIX)
	add_compile_definitions(CRYSTAL_USE_UNIX)
elseif(WIN32)
	add_compile_definitions(CRYSTAL_USE_WIN32)
else()
	message("Unsupported operating system")
endif()

add_compile_definitions(CRYSTAL_USE_FLOAT32 CRYSTAL_USE_GLFW)

include_directories(${PROJECT_SOURCE_DIR})

# 包含子项目。
add_subdirectory ("CrystalTracer")
add_subdirectory ("CrystalEngine")