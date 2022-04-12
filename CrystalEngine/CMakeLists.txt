cmake_minimum_required (VERSION 3.14)

project(CrystalEngine)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/external")
set(ENGINE_ASSET_DIR "assets")
set(BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/")


# set cmake compile options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
endif()


# graphics interface
option(CRYSTAL_USE_OPENGL "Use OpenGL for graphics interface" OFF)
option(CRYSTAL_USE_DX11 "Use DX11 and Win32 for graphics interface" ON)

# platform interface
option(CRYSTAL_USE_GLFW "Use GLFW for system platform provider" OFF)
option(CRYSTAL_USE_WIN32 "Use WIN32 for system platform provider" ON)

# other options
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

if (CRYSTAL_USE_GLFW)
    add_compile_definitions(CRYSTAL_USE_GLFW)
endif()
if (CRYSTAL_USE_WIN32)
    add_compile_definitions(CRYSTAL_USE_WIN32)
endif()

if (CRYSTAL_USE_DEBUG)
    add_compile_definitions(CRYSTAL_USE_DEBUG)
endif()

add_subdirectory ("external")
add_subdirectory ("src/Runtime")
add_subdirectory ("src/Editor")
add_subdirectory ("test")

