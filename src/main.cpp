// main.cpp
// Entry point for B-Lec game prototype
// Integrates window management, input handling, 3D rendering, block system, debug overlay, and UI

#include "window/window_manager.h"
#include "input/input_handler.h"
#include "render/renderer.h"
#include "render/font.h"
#include "render/camera.h"
#include "render/mesh.h"
#include "debug/debug_overlay.h"
#include "world/block_system.h"
#include "ui/ui_manager.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <cstdio>

namespace {

// Window configuration
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "B-Lec Prototype";

// GLFW error callback
void GLFWErrorCallback(int error, const char* description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

} // anonymous namespace

int main() {
    // Set up GLFW error callback
    glfwSetErrorCallback(GLFWErrorCallback);

    // Initialize all systems
    blec::window::WindowManager window_manager;
    blec::input::InputHandler input_handler;
    blec::render::Renderer renderer;
    blec::render::BitmapFont font;
    blec::render::Camera camera;
    blec::debug::DebugOverlay debug_overlay;
    blec::ui::UIManager ui_manager;

    // Initialize GLFW and create window
    if (!window_manager.InitializeGLFW()) {
        return 1;
    }

    if (!window_manager.CreateWindow(kWindowWidth, kWindowHeight, kWindowTitle)) {
        return 1;
    }

    window_manager.MakeContextCurrent();
    window_manager.SetVSync(1); // Enable VSync

    // Initialize renderer
    renderer.Initialize();

    // Initialize UI manager with window dimensions
    ui_manager.Initialize(kWindowWidth, kWindowHeight);

    // Initialize camera
    camera.Initialize(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    camera.SetMovementSpeed(5.0f);  // 5 units per second
    camera.SetRotationSpeed(0.005f); // radians per pixel

    // Create cube mesh
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    cube.SetBackfaceCulling(true);  // Only render front-facing faces

    // Initialize block system
    blec::world::BlockSystem block_system;
    block_system.Initialize(32, 32, 32, 1.0f);  // 32x32x32 grid with 1 unit blocks
    block_system.CreateTestBlocks();  // Create initial test block cube at center

    // Register input callbacks
    input_handler.RegisterCallbacks(window_manager.GetHandle());

    // Main loop timing
    using clock = std::chrono::steady_clock;
    auto last_frame_time = clock::now();

    // Static for toggle states
    static bool f12_was_down = false;
    static bool esc_was_down = false;

    // Main game loop
    while (!window_manager.ShouldClose()) {
        // Calculate delta time
        auto current_time = clock::now();
        std::chrono::duration<double> delta_duration = current_time - last_frame_time;
        double delta_time = delta_duration.count();
        last_frame_time = current_time;

        // Poll for events
        window_manager.PollEvents();

        // Handle ESC key to toggle pause (press detection)
        if (input_handler.IsKeyDown(GLFW_KEY_ESCAPE)) {
            if (!esc_was_down) {
                ui_manager.TogglePause();
                esc_was_down = true;
            }
        } else {
            esc_was_down = false;
        }

        // Update mouse lock state based on pause state
        GLFWwindow* window_handle = window_manager.GetHandle();
        if (ui_manager.IsPaused()) {
            // Unlock mouse when paused
            glfwSetInputMode(window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            // Lock mouse to window center when playing
            glfwSetInputMode(window_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        // Handle pause menu interactions (mouse clicks)
        if (ui_manager.IsPaused()) {
            // Get mouse position in screen coordinates
            double mouse_x = 0.0;
            double mouse_y = 0.0;
            glfwGetCursorPos(window_handle, &mouse_x, &mouse_y);
            
            // Check for mouse click
            if (input_handler.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
                auto action = ui_manager.HandleMouseClick(static_cast<float>(mouse_x), 
                                                          static_cast<float>(mouse_y));
                
                if (action == blec::ui::UIManager::ButtonAction::Resume) {
                    ui_manager.TogglePause();
                } else if (action == blec::ui::UIManager::ButtonAction::Quit) {
                    window_manager.SetShouldClose(true);
                }
            }
        }

        // Toggle debug overlay with F12 (press detection)
        if (input_handler.IsKeyDown(GLFW_KEY_F12)) {
            if (!f12_was_down) {
                debug_overlay.Toggle();
                f12_was_down = true;
            }
        } else {
            f12_was_down = false;
        }

        // Handle camera and gameplay only when not paused
        if (!ui_manager.IsPaused()) {
            // Handle camera movement (WASD keys)
            if (input_handler.IsKeyDown(GLFW_KEY_W) || input_handler.IsKeyDown(GLFW_KEY_UP)) {
                camera.MoveForward(static_cast<float>(delta_time * 5.0));
            }
            if (input_handler.IsKeyDown(GLFW_KEY_S) || input_handler.IsKeyDown(GLFW_KEY_DOWN)) {
                camera.MoveForward(static_cast<float>(-delta_time * 5.0));
            }
            if (input_handler.IsKeyDown(GLFW_KEY_A) || input_handler.IsKeyDown(GLFW_KEY_LEFT)) {
                camera.MoveRight(static_cast<float>(-delta_time * 5.0));
            }
            if (input_handler.IsKeyDown(GLFW_KEY_D) || input_handler.IsKeyDown(GLFW_KEY_RIGHT)) {
                camera.MoveRight(static_cast<float>(delta_time * 5.0));
            }
            if (input_handler.IsKeyDown(GLFW_KEY_SPACE)) {
                camera.MoveUp(static_cast<float>(delta_time * 5.0));
            }
            if (input_handler.IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
                camera.MoveUp(static_cast<float>(-delta_time * 5.0));
            }

            // Handle camera rotation (mouse look)
            double mouse_dx = 0.0;
            double mouse_dy = 0.0;
            input_handler.GetMouseLookDelta(&mouse_dx, &mouse_dy);

            // Some mice might be inverted, standard is positive Y = look up
            camera.Yaw(static_cast<float>(mouse_dx * 0.005f));
            camera.Pitch(static_cast<float>(-mouse_dy * 0.005f));  // Inverted for natural look
        }

        // Update camera and debug overlay
        camera.Update(delta_time);
        debug_overlay.Update(input_handler, delta_time);

        // Get framebuffer size for rendering
        int fb_width = 0;
        int fb_height = 0;
        window_manager.GetFramebufferSize(&fb_width, &fb_height);

        // Set viewport and clear screen
        renderer.SetViewport(fb_width, fb_height);
        renderer.Clear(0.1f, 0.15f, 0.2f, 1.0f); // Dark blue background

        // ===== UPDATE BLOCK SYSTEM =====
        // Create projection matrix for frustum extraction
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                static_cast<float>(fb_width) / fb_height,
                                                0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // Extract camera frustum and update block visibility
        block_system.ExtractFrustum(view, projection);
        block_system.UpdateVisibility();

        // Update debug overlay with camera and block information
        glm::vec3 cam_pos = camera.GetPosition();
        debug_overlay.SetCameraPosition(cam_pos.x, cam_pos.y, cam_pos.z);
        debug_overlay.SetCameraOrientation(camera.GetYaw(), camera.GetPitch());
        
        // Set block counts
        debug_overlay.SetBlockCounts(block_system.GetTotalBlockCount(),
                                     block_system.GetVisibleBlockCount());

        // ===== RENDER 3D SCENE =====
        renderer.Begin3D(fb_width, fb_height, 45.0f);

        // Set view matrix from camera
        renderer.SetView(view);

        // Render cube at origin
        glm::mat4 model = glm::mat4(1.0f);  // Identity matrix (cube at origin)
        renderer.SetModel(model);

        // Enable back-face culling
        renderer.EnableBackfaceCulling();

        // Render the cube
        cube.Render();

        // Disable back-face culling before 2D
        renderer.DisableBackfaceCulling();

        renderer.End3D();

        // ===== RENDER 2D OVERLAY =====
        renderer.Begin2D(fb_width, fb_height);

        // Render crosshair (always visible)
        ui_manager.RenderCrosshair(renderer);

        // Render pause menu (only when paused)
        ui_manager.RenderPauseMenu(renderer, font);

        // Render debug overlay if visible
        if (debug_overlay.IsVisible()) {
            debug_overlay.Render(fb_width, fb_height, font, input_handler);
        }

        renderer.End2D();

        // Swap buffers to display rendered frame
        window_manager.SwapBuffers();

        // Reset per-frame input state
        input_handler.ResetMouseDelta();
    }

    // Shutdown
    window_manager.Shutdown();
    return 0;
}
