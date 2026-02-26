// main.cpp
// Entry point for B-Lec game prototype
// Integrates window management, input handling, rendering, and debug overlay

#include "window/window_manager.h"
#include "input/input_handler.h"
#include "render/renderer.h"
#include "render/font.h"
#include "debug/debug_overlay.h"

#include <GLFW/glfw3.h>
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
    blec::debug::DebugOverlay debug_overlay;

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

    // Register input callbacks
    input_handler.RegisterCallbacks(window_manager.GetHandle());

    // Main loop timing
    using clock = std::chrono::steady_clock;
    auto last_frame_time = clock::now();

    // Main game loop
    while (!window_manager.ShouldClose()) {
        // Calculate delta time
        auto current_time = clock::now();
        std::chrono::duration<double> delta_duration = current_time - last_frame_time;
        double delta_time = delta_duration.count();
        last_frame_time = current_time;

        // Poll for events
        window_manager.PollEvents();

        // Check for ESC key to close window
        if (input_handler.IsKeyDown(GLFW_KEY_ESCAPE)) {
            window_manager.SetShouldClose(true);
        }

        // Toggle debug overlay with F12
        if (input_handler.IsKeyDown(GLFW_KEY_F12)) {
            // Note: This will toggle every frame while held
            // Could add press/release tracking if needed
            static bool f12_was_down = false;
            if (!f12_was_down) {
                debug_overlay.Toggle();
                f12_was_down = true;
            }
        } else {
            static bool f12_was_down = false;
            f12_was_down = false;
        }

        // Update debug overlay
        debug_overlay.Update(input_handler, delta_time);

        // Get framebuffer size for rendering
        int fb_width = 0;
        int fb_height = 0;
        window_manager.GetFramebufferSize(&fb_width, &fb_height);

        // Set viewport and clear screen
        renderer.SetViewport(fb_width, fb_height);
        renderer.Clear(0.08f, 0.1f, 0.12f, 1.0f); // Dark blue-gray background

        // Render debug overlay if visible
        if (debug_overlay.IsVisible()) {
            debug_overlay.Render(fb_width, fb_height, font, input_handler);
        }

        // Swap buffers to display rendered frame
        window_manager.SwapBuffers();

        // Reset per-frame input state
        input_handler.ResetMouseDelta();
    }

    // Shutdown
    window_manager.Shutdown();
    return 0;
}
