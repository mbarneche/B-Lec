// input_handler.cpp
// Implementation of input tracking and callback handling

#include "input/input_handler.h"
#include <GLFW/glfw3.h>
#include <vector>

namespace blec {
namespace input {

// Static callback wrappers that forward to instance methods
static InputHandler* g_input_handler = nullptr;

static void KeyCallbackWrapper(GLFWwindow*, int key, int scancode, int action, int mods) {
    if (g_input_handler) {
        g_input_handler->OnKey(key, scancode, action, mods);
    }
}

static void CursorPosCallbackWrapper(GLFWwindow*, double xpos, double ypos) {
    if (g_input_handler) {
        g_input_handler->OnCursorPos(xpos, ypos);
    }
}

InputHandler::InputHandler()
    : key_down_{}
    , last_key_event_()
    , mouse_x_(0.0)
    , mouse_y_(0.0)
    , mouse_dx_(0.0)
    , mouse_dy_(0.0)
    , has_mouse_pos_(false) {
}

void InputHandler::RegisterCallbacks(GLFWwindow* window) {
    g_input_handler = this;
    glfwSetKeyCallback(window, KeyCallbackWrapper);
    glfwSetCursorPosCallback(window, CursorPosCallbackWrapper);
}

bool InputHandler::IsKeyDown(int key) const {
    if (key >= 0 && key < static_cast<int>(key_down_.size())) {
        return key_down_[key];
    }
    return false;
}

std::string InputHandler::GetKeyName(int key) {
    // Alphabetic keys
    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        char c = static_cast<char>('A' + (key - GLFW_KEY_A));
        return std::string(1, c);
    }

    // Numeric keys
    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        char c = static_cast<char>('0' + (key - GLFW_KEY_0));
        return std::string(1, c);
    }

    // Special keys
    switch (key) {
        case GLFW_KEY_SPACE:            return "Space";
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:      return "Shift";
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:    return "Ctrl";
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:        return "Alt";
        case GLFW_KEY_UP:               return "Up";
        case GLFW_KEY_DOWN:             return "Down";
        case GLFW_KEY_LEFT:             return "Left";
        case GLFW_KEY_RIGHT:            return "Right";
        case GLFW_KEY_ESCAPE:           return "Esc";
        case GLFW_KEY_ENTER:            return "Enter";
        case GLFW_KEY_TAB:              return "Tab";
        case GLFW_KEY_BACKSPACE:        return "Backspace";
        case GLFW_KEY_F12:              return "F12";
        default:                        return "Key";
    }
}

const char* InputHandler::GetActionName(int action) {
    switch (action) {
        case GLFW_PRESS:    return "press";
        case GLFW_RELEASE:  return "release";
        case GLFW_REPEAT:   return "repeat";
        default:            return "unknown";
    }
}

std::string InputHandler::GetKeysDownString() const {
    std::vector<std::string> keys;

    for (size_t key = 0; key < key_down_.size(); ++key) {
        if (key_down_[key]) {
            keys.push_back(GetKeyName(static_cast<int>(key)));
        }
    }

    if (keys.empty()) {
        return "(none)";
    }

    std::string result;
    for (size_t i = 0; i < keys.size(); ++i) {
        result += keys[i];
        if (i + 1 < keys.size()) {
            result += ", ";
        }
    }
    return result;
}

void InputHandler::GetMousePosition(double* x, double* y) const {
    if (x) *x = mouse_x_;
    if (y) *y = mouse_y_;
}

void InputHandler::GetMouseDelta(double* dx, double* dy) const {
    if (dx) *dx = mouse_dx_;
    if (dy) *dy = mouse_dy_;
}

void InputHandler::GetMouseLookDelta(double* dx, double* dy) const {
    // Same as GetMouseDelta, but semantically for camera look
    GetMouseDelta(dx, dy);
}

void InputHandler::ResetMouseDelta() {
    mouse_dx_ = 0.0;
    mouse_dy_ = 0.0;
}

void InputHandler::OnKey(int key, int, int action, int) {
    // Update key state
    if (key >= 0 && key < static_cast<int>(key_down_.size())) {
        if (action == GLFW_PRESS) {
            key_down_[key] = true;
        } else if (action == GLFW_RELEASE) {
            key_down_[key] = false;
        }
    }

    // Record the event
    last_key_event_ = GetKeyName(key) + " " + GetActionName(action);
}

void InputHandler::OnCursorPos(double xpos, double ypos) {
    if (has_mouse_pos_) {
        mouse_dx_ = xpos - mouse_x_;
        mouse_dy_ = ypos - mouse_y_;
    } else {
        mouse_dx_ = 0.0;
        mouse_dy_ = 0.0;
        has_mouse_pos_ = true;
    }

    mouse_x_ = xpos;
    mouse_y_ = ypos;
}

} // namespace input
} // namespace blec
