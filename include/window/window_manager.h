// window_manager.h
// Manages GLFW window creation, lifecycle, and properties
// Cross-platform window management for Windows, Linux, and macOS

#ifndef BLEC_WINDOW_MANAGER_H
#define BLEC_WINDOW_MANAGER_H

struct GLFWwindow;

namespace blec {
namespace window {

// WindowManager handles window creation and basic window operations
// Encapsulates GLFW window lifecycle
class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    // Initialize GLFW library
    // Returns true on success, false on failure
    bool InitializeGLFW();

    // Create a window with the specified dimensions and title
    // Returns true on success, false on failure
    bool CreateWindow(int width, int height, const char* title);

    // Make the window's OpenGL context current
    void MakeContextCurrent();

    // Enable or disable VSync (1 = enabled, 0 = disabled)
    void SetVSync(int interval);

    // Check if the window should close (ESC pressed or window closed)
    bool ShouldClose() const;

    // Mark window as should close
    void SetShouldClose(bool shouldClose);

    // Poll for window and input events
    void PollEvents();

    // Swap front and back buffers (display rendered frame)
    void SwapBuffers();

    // Get current framebuffer dimensions (for OpenGL viewport)
    void GetFramebufferSize(int* width, int* height) const;

    // Get the GLFW window handle (for callbacks)
    GLFWwindow* GetHandle() const { return window_; }

    // Cleanup and destroy window and terminate GLFW
    void Shutdown();

private:
    GLFWwindow* window_;
    bool glfw_initialized_;

    // Non-copyable
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};

} // namespace window
} // namespace blec

#endif // BLEC_WINDOW_MANAGER_H
