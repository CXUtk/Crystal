# CMakeList.txt: Crystal 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

file(GLOB CrystalEngineSources_Extra "src/*.cpp")
set(CrystalEngineSources_PlatformFactory "src/Platforms/PlatformFactory.cpp" "src/Platforms/PlatformFactory.h")
file(GLOB_RECURSE CrystalEngineSources_Core "src/Core/*.cpp")
file(GLOB_RECURSE CrystalEngineHeaders_Core "src/Core/*.h")
file(GLOB_RECURSE CrystalEngineTests "src/Tests/*.cpp")

if (CRYSTAL_USE_OPENGL MATCHES ON AND CRYSTAL_USE_DX11 MATCHES ON)
    message(FATAL_ERROR "CRYSTAL_USE_GLFW and CRYSTAL_USE_DX11 cannot both be true")
endif()

if (CRYSTAL_USE_OPENGL)
    file(GLOB_RECURSE CrystalEngineSources_Platform "src/Platforms/OpenGL/*.cpp")
    file(GLOB_RECURSE tmp "src/Platforms/GLFW/*.cpp")
    list(APPEND CrystalEngineSources_Platform ${tmp})
endif()

if (CRYSTAL_USE_DX11)
    file(GLOB_RECURSE CrystalEngineSources_Platform "src/Platforms/DX11/*.cpp")
    file(GLOB_RECURSE tmp "src/Platforms/Windows32/*.cpp")
    list(APPEND CrystalEngineSources_Platform ${tmp})
endif()

# 将源代码添加到此项目的可执行文件。
add_library (CrystalEngine
${CrystalEngineSources_Extra} 
${CrystalEngineSources_Core}
${CrystalEngineHeaders_Core}
${CrystalEngineSources_Platform}
${CrystalEngineSources_PlatformFactory}
${CrystalEngineTests})

set_target_properties(CrystalEngine PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

# 调整GLFW的Flags，关掉无用的编译组件
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# 操作系统和图形接口判定
add_subdirectory("external/SJson")
target_link_libraries(CrystalEngine PRIVATE SJson)

if (CRYSTAL_USE_OPENGL)
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

    find_package(OpenGL REQUIRED)
    target_link_libraries(CrystalEngine PRIVATE OpenGL::GL)
    target_link_libraries(CrystalEngine PRIVATE glad)
endif()

if (CRYSTAL_USE_DX11)
    target_link_libraries(CrystalEngine PRIVATE d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)
endif()

target_include_directories(CrystalEngine SYSTEM PUBLIC "external")
target_include_directories(CrystalEngine SYSTEM PUBLIC "src")

# TODO: 如有需要，请添加测试并安装目标。