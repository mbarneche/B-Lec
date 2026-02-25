/**
 * @file input_manager.cpp
 * @brief Input manager implementation
 */

#include "input/input_manager.h"
#include <GLFW/glfw3.h>

InputManager::InputManager(GLFWwindow* window)
    : window_(window), mouse_position_(0.0f, 0.0f), 
      previous_mouse_position_(0.0f, 0.0f), mouse_delta_(0.0f, 0.0f) {
    std::fill(current_keys_.begin(), current_keys_.end(), false);
    std::fill(previous_keys_.begin(), previous_keys_.end(), false);
    std::fill(current_mouse_buttons_.begin(), current_mouse_buttons_.end(), false);
    std::fill(previous_mouse_buttons_.begin(), previous_mouse_buttons_.end(), false);
}

InputManager::~InputManager() {
}

void InputManager::Update() {
    previous_keys_ = current_keys_;
    previous_mouse_position_ = mouse_position_;
    previous_mouse_buttons_ = current_mouse_buttons_;

    // Update key states
    current_keys_[static_cast<int>(Key::W)] = glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS;
    current_keys_[static_cast<int>(Key::A)] = glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS;
    current_keys_[static_cast<int>(Key::S)] = glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS;
    current_keys_[static_cast<int>(Key::D)] = glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS;
    current_keys_[static_cast<int>(Key::SPACE)] = glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS;
    current_keys_[static_cast<int>(Key::SHIFT)] = glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    current_keys_[static_cast<int>(Key::E)] = glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS;
    current_keys_[static_cast<int>(Key::Q)] = glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS;
    current_keys_[static_cast<int>(Key::ESC)] = glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    // Update mouse button states
    current_mouse_buttons_[0] = glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    current_mouse_buttons_[1] = glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    current_mouse_buttons_[2] = glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

    // Get mouse position
    double x, y;
    glfwGetCursorPos(window_, &x, &y);
    mouse_position_ = glm::vec2(x, y);
    mouse_delta_ = mouse_position_ - previous_mouse_position_;
}

bool InputManager::IsKeyPressed(Key key) const {
    return current_keys_[static_cast<int>(key)];
}

bool InputManager::IsKeyJustPressed(Key key) const {
    int index = static_cast<int>(key);
    return current_keys_[index] && !previous_keys_[index];
}

bool InputManager::IsMouseButtonPressed(uint32_t button) const {
    if (button >= 3) return false;
    return current_mouse_buttons_[button];
}

bool InputManager::IsMouseButtonJustPressed(uint32_t button) const {
    if (button >= 3) return false;
    return current_mouse_buttons_[button] && !previous_mouse_buttons_[button];
}

void InputManager::SetMousePosition(double x, double y) {
    glfwSetCursorPos(window_, x, y);
}

bool InputManager::WindowCloseRequested() const {
    return glfwWindowShouldClose(window_);
}

void InputManager::CenterMouse(uint32_t window_width, uint32_t window_height) {
    SetMousePosition(window_width / 2.0, window_height / 2.0);
}
