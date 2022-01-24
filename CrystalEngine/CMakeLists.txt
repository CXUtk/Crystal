# CMakeList.txt: Crystal 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE CrystalEngineSources "src/*.cpp")
file(GLOB_RECURSE CrystalEngineHeaders "src/*.h")

# 将源代码添加到此项目的可执行文件。
add_library (CrystalEngine ${CrystalEngineSources} ${CrystalEngineHeaders})

# 调整GLFW的Flags，关掉无用的编译组件
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# 操作系统判定
if (UNIX)
    FIND_PACKAGE (glfw3)
    if (glfw3_FOUND)
        target_link_libraries(CrystalEngine PRIVATE glfw)
    else()
        add_subdirectory("external/GLFW")
        target_link_libraries(CrystalEngine PRIVATE glfw)
    endif()
    target_link_libraries (CrystalEngine PRIVATE dl X11 pthread)
else()
    add_subdirectory("external/GLFW")
    target_link_libraries(CrystalEngine PRIVATE glfw)
endif()

add_subdirectory("external/glad")
add_subdirectory("external/SJson")

find_package(OpenGL REQUIRED)
target_link_libraries(CrystalEngine PRIVATE OpenGL::GL)
target_link_libraries(CrystalEngine PRIVATE glad SJson)

target_include_directories(CrystalEngine SYSTEM PUBLIC "external")
target_include_directories(CrystalEngine PUBLIC "src")

# TODO: 如有需要，请添加测试并安装目标。
