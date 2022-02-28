cmake_minimum_required (VERSION 3.11)


file(GLOB CrystalEngineSources_Extra "*.cpp")
set(CrystalEngineSources_PlatformFactory "Platforms/PlatformFactory.cpp" "Platforms/PlatformFactory.h")
file(GLOB_RECURSE CrystalEngineSources_Core "Core/*.cpp")
file(GLOB_RECURSE CrystalEngineInterfaces_Core "Interfaces/*.cpp")
file(GLOB_RECURSE CrystalEngineTests "Tests/*.cpp")

if (CRYSTAL_USE_OPENGL MATCHES ON AND CRYSTAL_USE_DX11 MATCHES ON)
    message(FATAL_ERROR "CRYSTAL_USE_GLFW and CRYSTAL_USE_DX11 cannot both be true")
endif()

if (CRYSTAL_USE_OPENGL)
    file(GLOB_RECURSE CrystalEngineSources_Platform "Platforms/OpenGL/*.cpp")
    file(GLOB_RECURSE tmp "Platforms/GLFW/*.cpp")
    list(APPEND CrystalEngineSources_Platform ${tmp})
endif()

if (CRYSTAL_USE_DX11)
    file(GLOB_RECURSE CrystalEngineSources_Platform "Platforms/DX11/*.cpp")
    file(GLOB_RECURSE tmp "Platforms/Windows32/*.cpp")
    list(APPEND CrystalEngineSources_Platform ${tmp})
endif()


# ½«Ô´´úÂëÌí¼Óµ½´ËÏîÄ¿µÄ¿ÉÖ´ÐÐÎÄ¼þ¡£
add_library (CrystalEngine
${CrystalEngineSources_Extra} 
${CrystalEngineSources_Core}
${CrystalEngineInterfaces_Core}
${CrystalEngineSources_Platform}
${CrystalEngineSources_PlatformFactory}
${CrystalEngineTests})

set_target_properties(CrystalEngine PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF)


# µ÷ÕûGLFWµÄFlags£¬¹ØµôÎÞÓÃµÄ±àÒë×é¼þ
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# ²Ù×÷ÏµÍ³ºÍÍ¼ÐÎ½Ó¿ÚÅÐ¶¨

if (CRYSTAL_USE_OPENGL)
    if (UNIX)
        FIND_PACKAGE (glfw3)
        if (glfw3_FOUND)
            target_link_libraries(CrystalEngine PRIVATE glfw)
        else()
            add_subdirectory("../external/GLFW")
            target_link_libraries(CrystalEngine PRIVATE glfw)
        endif()
        target_link_libraries (CrystalEngine PRIVATE dl X11 pthread)
    else()
        add_subdirectory("../external/GLFW")
        target_link_libraries(CrystalEngine PRIVATE glfw)
    endif()
    add_subdirectory("../external/glad")

    find_package(OpenGL REQUIRED)
    target_link_libraries(CrystalEngine PRIVATE OpenGL::GL)
    target_link_libraries(CrystalEngine PRIVATE glad)
endif()

if (CRYSTAL_USE_DX11)
    target_link_libraries(CrystalEngine PRIVATE d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)
endif()

target_include_directories(CrystalEngine SYSTEM PUBLIC "../external")
target_include_directories(CrystalEngine SYSTEM PUBLIC ".")