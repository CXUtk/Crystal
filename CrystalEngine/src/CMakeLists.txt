cmake_minimum_required (VERSION 3.14)


file(GLOB CrystalEngineSources_Extra CONFIGURE_DEPENDS "*.cpp")
set(CrystalEngineSources_PlatformFactory "Platforms/PlatformFactory.cpp" "Platforms/PlatformFactory.h")
file(GLOB_RECURSE CrystalEngineSources_Core CONFIGURE_DEPENDS "Core/*.cpp")
file(GLOB_RECURSE CrystalEngineInterfaces_Core CONFIGURE_DEPENDS "Interfaces/*.cpp")
file(GLOB_RECURSE CrystalEngineTests CONFIGURE_DEPENDS "Tests/*.cpp")

if (CRYSTAL_USE_OPENGL MATCHES ON AND CRYSTAL_USE_DX11 MATCHES ON)
    message(FATAL_ERROR "CRYSTAL_USE_GLFW and CRYSTAL_USE_DX11 cannot both be true")
endif()

if (CRYSTAL_USE_OPENGL)
    file(GLOB_RECURSE CrystalEngineSources_Platform CONFIGURE_DEPENDS "Platforms/OpenGL/*.cpp")
    file(GLOB_RECURSE tmp "Platforms/GLFW/*.cpp")
    list(APPEND CrystalEngineSources_Platform ${tmp})
endif()

if (CRYSTAL_USE_DX11)
    file(GLOB_RECURSE CrystalEngineSources_Platform CONFIGURE_DEPENDS "Platforms/DX11/*.cpp")
    file(GLOB_RECURSE tmp "Platforms/Windows32/*.cpp")
    list(APPEND CrystalEngineSources_Platform ${tmp})
endif()



add_library (CrystalEngine
${CrystalEngineSources_Extra} 
${CrystalEngineSources_Core}
${CrystalEngineInterfaces_Core}
${CrystalEngineSources_Platform}
${CrystalEngineSources_PlatformFactory}
${CrystalEngineTests})

set_target_properties(CrystalEngine PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED TRUE
)



set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


if (CRYSTAL_USE_OPENGL)
    if (UNIX)
        FIND_PACKAGE (glfw3)
        if (glfw3_FOUND)
            target_link_libraries(CrystalEngine PRIVATE glfw)
        else()
            add_subdirectory("../external/includes/GLFW")
            target_link_libraries(CrystalEngine PRIVATE glfw)
        endif()
        target_link_libraries (CrystalEngine PRIVATE dl X11 pthread)
    else()
        add_subdirectory("../external/includes/GLFW")
        target_link_libraries(CrystalEngine PRIVATE glfw)
    endif()
    add_subdirectory("../external/includes/glad")

    find_package(OpenGL REQUIRED)
    target_link_libraries(CrystalEngine PRIVATE OpenGL::GL)
    target_link_libraries(CrystalEngine PRIVATE glad)
endif()

if (CRYSTAL_USE_DX11)
    target_link_libraries(CrystalEngine PRIVATE d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)
endif()

#target_link_directories(CrystalEngine PRIVATE "${PROJECT_SOURCE_DIR}/external/libs/freetype")
target_link_libraries(CrystalEngine PRIVATE ${PROJECT_SOURCE_DIR}/external/libs/freetype/freetype.lib)

target_include_directories(CrystalEngine SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/includes)
target_include_directories(CrystalEngine SYSTEM PUBLIC ".")