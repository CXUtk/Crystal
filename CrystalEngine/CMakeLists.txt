# CMakeList.txt: Crystal 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

set(CrystalEngineSources_Extra "src/Crystal.h" "src/Engine.cpp" "src/Engine.h")
set(CrystalEngineSources_PlatformFactory "src/Platforms/PlatformFactory.cpp" "src/Platforms/PlatformFactory.h")
file(GLOB_RECURSE CrystalEngineSources_Core "src/Core/*.cpp")

if (CRYSTAL_USE_GLFW MATCHES ON AND CRYSTAL_USE_DX11 MATCHES ON)
    message(FATAL_ERROR "CRYSTAL_USE_GLFW and CRYSTAL_USE_DX11 cannot both be true")
endif()

if (CRYSTAL_USE_GLFW)
    file(GLOB_RECURSE CrystalEngineSources_Platform "src/Platforms/GLFW/*.cpp")
    list(APPEND CrystalEngineSources_Platform "src/Platforms/GLFWPlatform.h")
endif()

if (CRYSTAL_USE_DX11)
    file(GLOB_RECURSE CrystalEngineSources_Platform "src/Platforms/DX11/*.cpp")
    file(GLOB_RECURSE tmp "src/Platforms/Windows32/*.cpp")
    list(APPEND CrystalEngineSources_Platform ${tmp})
    list(APPEND CrystalEngineSources_Platform "src/Platforms/DX11Platform.h")
endif()

message(${CrystalEngineSources_Platform})

# 将源代码添加到此项目的可执行文件。
add_library (CrystalEngine
${CrystalEngineSources_Extra} 
${CrystalEngineSources_Core}
${CrystalEngineSources_Platform}
${CrystalEngineSources_PlatformFactory})

set_target_properties(CrystalEngine PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

# 调整GLFW的Flags，关掉无用的编译组件
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


option(CRYSTAL_USE_GLFW "Use GLFW for platform" OFF)
option(CRYSTAL_USE_DX11 "Use DX11 for platform" ON)
option(CRYSTAL_USE_DEBUG "Enable debug infomation and outputs" ON)


if (UNIX)
	add_compile_definitions(CRYSTAL_USE_UNIX)
elseif(WIN32)
	add_compile_definitions(CRYSTAL_USE_WIN32)
else()
	message("Unsupported operating system")
endif()

add_compile_definitions(CRYSTAL_USE_FLOAT32)

if (CRYSTAL_USE_DEBUG)
    add_compile_definitions(CRYSTAL_USE_DEBUG)
endif()


# 操作系统和图形接口判定

if (CRYSTAL_USE_GLFW)
	add_compile_definitions(CRYSTAL_USE_GLFW)
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
    target_link_libraries(CrystalEngine PRIVATE glad SJson)
endif()

if (CRYSTAL_USE_DX11)
	add_compile_definitions(CRYSTAL_USE_DX11)
    target_link_libraries(CrystalEngine PRIVATE d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)
endif()

add_subdirectory("external/SJson")

target_include_directories(CrystalEngine SYSTEM PUBLIC "external")
target_include_directories(CrystalEngine SYSTEM PUBLIC "src")

# TODO: 如有需要，请添加测试并安装目标。
