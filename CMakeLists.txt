cmake_minimum_required(VERSION 3.20)
project(BLec LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(OpenGL REQUIRED)

# GLFW
if(NOT TARGET glfw)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ElectricitySimulator/external/glfw)
endif()

# GLM (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ElectricitySimulator/external/glm)

# Main executable
add_executable(blec 
    ElectricitySimulator/src/main.cpp
    ElectricitySimulator/src/application.cpp
    ElectricitySimulator/src/rendering/renderer.cpp
    ElectricitySimulator/src/rendering/shader.cpp
    ElectricitySimulator/src/world/world.cpp
    ElectricitySimulator/src/world/block.cpp
    ElectricitySimulator/src/world/chunk.cpp
    ElectricitySimulator/src/world/raycast.cpp
    ElectricitySimulator/src/world/block_interaction.cpp
    ElectricitySimulator/src/world/block_selector.cpp
    ElectricitySimulator/src/input/input_manager.cpp
    ElectricitySimulator/src/camera/camera.cpp
    ElectricitySimulator/src/ui/ui_system.cpp
    ElectricitySimulator/src/persistence/world_serializer.cpp
)

# Include directories
target_include_directories(blec PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ElectricitySimulator/include
    ${OPENGL_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(blec PRIVATE
    glfw
    OpenGL::GL
    glm
)

# Platform-specific settings
if(WIN32)
    set_target_properties(blec PROPERTIES WIN32_EXECUTABLE ON)
endif()

# Optional: Add compilation flags for warnings and optimization
if(MSVC)
    target_compile_options(blec PRIVATE /W4 /WX)
else()
    target_compile_options(blec PRIVATE -Wall -Wextra -Werror -O2)
endif()

# Define project version
add_definitions(-DBLEC_VERSION_MAJOR=0)
add_definitions(-DBLEC_VERSION_MINOR=1)
add_definitions(-DBLEC_VERSION_PATCH=0)
