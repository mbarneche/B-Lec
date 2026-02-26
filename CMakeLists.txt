cmake_minimum_required(VERSION 4.2)

project(BLecPrototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# GLFW for window and input management
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
)

# GLM for mathematics (header-only, no build needed)
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.2/glm-1.0.2.zip
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw glm)

find_package(OpenGL REQUIRED)

# Executable with all module source files
add_executable(blec
    src/main.cpp
    src/window/window_manager.cpp
    src/input/input_handler.cpp
    src/render/renderer.cpp
    src/render/font.cpp
    src/render/mesh.cpp
    src/render/camera.cpp
    src/debug/debug_overlay.cpp
    src/world/block_system.cpp
)

# Include directories for headers
target_include_directories(blec PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
)

target_link_libraries(blec PRIVATE glfw OpenGL::GL)

if (MSVC)
    target_compile_options(blec PRIVATE /W4 /permissive-)
else()
    target_compile_options(blec PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional: Build tests
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(code_testing)
endif()
