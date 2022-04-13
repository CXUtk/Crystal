cmake_minimum_required (VERSION 3.14)

set(TARGET_NAME "CrystalEngine_Runtime")

# set compile paths
set(ENGINE_CORE_DIR "Core/")
set(ENGINE_TOOL_DIR "Tool/")
set(ENGINE_FUNCTION_DIR "Function/")
set(ENGINE_PLATFORM_DIR "Platform/")
set(ENGINE_RESOURCE_DIR "Resource/")

# set 3rd party libraries paths

# Add interface files
set(ENGINE_PLATFORM_FACTORY
"${ENGINE_PLATFORM_DIR}/PlatformFactory.cpp"
"${ENGINE_PLATFORM_DIR}/PlatformFactory.h")

file(GLOB_RECURSE ENGINE_PLATFORM_RHI CONFIGURE_DEPENDS
"${ENGINE_PLATFORM_DIR}/RHI/*.cpp"
"${ENGINE_PLATFORM_DIR}/RHI/*.h")


message(${CRYSTAL_USE_DX11})

# Different platform options cannot all be on
if (CRYSTAL_USE_OPENGL MATCHES ON AND CRYSTAL_USE_DX11 MATCHES ON)
    message(FATAL_ERROR "CRYSTAL_USE_GLFW and CRYSTAL_USE_DX11 cannot both be true")
endif()

if (CRYSTAL_USE_OPENGL)
    file(GLOB_RECURSE ENGINE_PLATFORM_GRAPHIC CONFIGURE_DEPENDS
    "${ENGINE_PLATFORM_DIR}/Graphics/OpenGL/*.cpp"
    "${ENGINE_PLATFORM_DIR}/Graphics/OpenGL/*.h")

    file(GLOB_RECURSE ENGINE_PLATFORM_SYSTEM CONFIGURE_DEPENDS
    "${ENGINE_PLATFORM_DIR}/System/GLFW/*.cpp"
    "${ENGINE_PLATFORM_DIR}/System/GLFW/*.h")
endif()

if (CRYSTAL_USE_DX11)
    file(GLOB_RECURSE ENGINE_PLATFORM_GRAPHIC CONFIGURE_DEPENDS
    "${ENGINE_PLATFORM_DIR}/Graphics/DX11/*.cpp"
    "${ENGINE_PLATFORM_DIR}/Graphics/DX11/*.h")

    file(GLOB_RECURSE ENGINE_PLATFORM_SYSTEM CONFIGURE_DEPENDS
    "${ENGINE_PLATFORM_DIR}/System/Windows32/*.cpp"
    "${ENGINE_PLATFORM_DIR}/System/Windows32/*.h")
endif()

# Add compile sources
file(GLOB ENGINE_EXTRA_SRC CONFIGURE_DEPENDS "*.cpp" "*.h")
file(GLOB_RECURSE ENGINE_CORE_SRC CONFIGURE_DEPENDS "${ENGINE_CORE_DIR}/*.cpp" "${ENGINE_CORE_DIR}/*.h")
file(GLOB_RECURSE ENGINE_TOOL_SRC CONFIGURE_DEPENDS "${ENGINE_TOOL_DIR}/*.cpp" "${ENGINE_TOOL_DIR}/*.h")
file(GLOB_RECURSE ENGINE_FUNCTION_SRC CONFIGURE_DEPENDS "${ENGINE_FUNCTION_DIR}/*.cpp" "${ENGINE_FUNCTION_DIR}/*.h")
file(GLOB_RECURSE ENGINE_RESOURCE_SRC CONFIGURE_DEPENDS "${ENGINE_RESOURCE_DIR}/*.cpp" "${ENGINE_RESOURCE_DIR}/*.h")

add_library (${TARGET_NAME} STATIC
${ENGINE_EXTRA_SRC}
${ENGINE_CORE_SRC}
${ENGINE_TOOL_SRC}
${ENGINE_FUNCTION_SRC}
${ENGINE_RESOURCE_SRC}
${ENGINE_PLATFORM_FACTORY} 
${ENGINE_PLATFORM_RHI}
${ENGINE_PLATFORM_GRAPHIC}
${ENGINE_PLATFORM_SYSTEM} ${CPP_HINT_DIR})

# set compile options
set_target_properties(${TARGET_NAME} PROPERTIES
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
CXX_EXTENSIONS OFF)

if(MSVC)
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
endif()

set_target_properties(${TARGET_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# link libraries
target_link_libraries(${TARGET_NAME} PUBLIC freetype)
target_link_libraries(${TARGET_NAME} PUBLIC glm)
target_link_libraries(${TARGET_NAME} PUBLIC utf8cpp)


if (CRYSTAL_USE_OPENGL)
    target_link_libraries(${TARGET_NAME} PUBLIC glad)
    target_link_libraries(${TARGET_NAME} PUBLIC glfw)

    find_package(OpenGL REQUIRED)
    target_link_libraries(${TARGET_NAME} PUBLIC OpenGL::GL)


    target_include_directories(${TARGET_NAME} PUBLIC ${THIRD_PARTY_DIR}/GLFW/include)
endif()

if (CRYSTAL_USE_DX11)
    target_link_libraries(${TARGET_NAME} PUBLIC d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${ENGINE_ROOT_DIR}/src/Runtime)
target_include_directories(${TARGET_NAME} PUBLIC ${THIRD_PARTY_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${THIRD_PARTY_DIR}/freetype/include)