cmake_minimum_required (VERSION 3.14)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if(CRYSTAL_USE_OPENGL AND NOT TARGET glad)
    add_subdirectory("glad")
endif()

if(CRYSTAL_USE_OPENGL AND NOT TARGET glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory("GLFW3")
endif()


if(NOT TARGET glm)
    option(BUILD_STATIC_LIBS "" ON)
    option(BUILD_TESTING "" OFF)
    option(GLM_TEST_ENABLE "" OFF)
    add_subdirectory("glm")
endif()

if(NOT TARGET GTest::gtest)
    add_subdirectory("gtest")
endif()

if(NOT TARGET freetype)
    add_subdirectory("freetype1.12")
endif()


if(NOT TARGET utf8cpp)
    add_subdirectory("utfcpp-3.2.1")
endif()
