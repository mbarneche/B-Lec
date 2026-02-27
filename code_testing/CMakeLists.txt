# CMakeLists.txt for B-Lec Unit Tests
# Builds individual test executables for each module

cmake_minimum_required(VERSION 3.20)

# Test executables
set(TEST_SOURCES
    window/test_window_manager.cpp
    input/test_input_handler.cpp
    render/test_font.cpp
    render/test_renderer.cpp
    render/test_camera.cpp
    render/test_mesh.cpp
    render/test_renderer_3d.cpp
    debug/test_debug_overlay.cpp
    world/test_block_system.cpp
    ui/test_ui_manager.cpp
)

# Build each test as a separate executable
set(TEST_TARGETS)

foreach(TEST_SOURCE ${TEST_SOURCES})
    # Extract test name from path
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    
    # Create test executable
    add_executable(${TEST_NAME}
        ${TEST_SOURCE}
        ../src/window/window_manager.cpp
        ../src/input/input_handler.cpp
        ../src/render/renderer.cpp
        ../src/render/font.cpp
        ../src/render/camera.cpp
        ../src/render/mesh.cpp
        ../src/debug/debug_overlay.cpp
        ../src/world/block_system.cpp
        ../src/ui/ui_manager.cpp
    )
    
    # Include directories
    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${glm_SOURCE_DIR}
    )
    
    # Link libraries
    target_link_libraries(${TEST_NAME} PRIVATE
        glfw
        OpenGL::GL
    )
    
    # Set output directory
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )

    list(APPEND TEST_TARGETS ${TEST_NAME})
endforeach()

# Optional: Add custom target to run all tests
add_custom_target(run_tests
    COMMAND ${CMAKE_COMMAND} -E echo "Running all tests..."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running unit tests"
)

add_dependencies(run_tests ${TEST_TARGETS})

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_custom_command(TARGET run_tests POST_BUILD
        COMMAND $<TARGET_FILE:${TEST_NAME}>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running ${TEST_NAME}"
    )
endforeach()
