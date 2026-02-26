// input_handler.h
// Manages keyboard and mouse input state and callbacks
// Tracks key presses, releases, and mouse movement

#ifndef BLEC_INPUT_HANDLER_H
#define BLEC_INPUT_HANDLER_H

#include <array>
#include <string>

struct GLFWwindow;

namespace blec {
namespace input {

// InputHandler tracks all keyboard and mouse input state
// Provides callbacks for GLFW and query methods for input state
class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    // Register callbacks with GLFW window
    void RegisterCallbacks(GLFWwindow* window);

    // Check if a specific key is currently pressed
    bool IsKeyDown(int key) const;

    // Get a human-readable name for a key code
    static std::string GetKeyName(int key);

    // Get action name (press, release, repeat)
    static const char* GetActionName(int action);

    // Get formatted string of all currently pressed keys
    std::string GetKeysDownString() const;

    // Get the last key event as a string (e.g., "W press")
    const std::string& GetLastKeyEvent() const { return last_key_event_; }

    // Get current mouse position
    void GetMousePosition(double* x, double* y) const;

    // Get mouse delta (movement since last frame)
    void GetMouseDelta(double* dx, double* dy) const;

    // Get raw mouse movement for camera look control
    // Same as GetMouseDelta but typically used for FPS camera
    void GetMouseLookDelta(double* dx, double* dy) const;

    // Reset mouse delta (call once per frame after using it)
    void ResetMouseDelta();

    // GLFW callback handlers (called by GLFW via static wrappers)
    void OnKey(int key, int scancode, int action, int mods);
    void OnCursorPos(double xpos, double ypos);

private:
    // Key state array (true = pressed, false = released)
    std::array<bool, 348> key_down_; // GLFW_KEY_LAST + 1

    // Last recorded key event
    std::string last_key_event_;

    // Mouse state
    double mouse_x_;
    double mouse_y_;
    double mouse_dx_;
    double mouse_dy_;
    bool has_mouse_pos_;

    // Non-copyable
    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;
};

} // namespace input
} // namespace blec

#endif // BLEC_INPUT_HANDLER_H
