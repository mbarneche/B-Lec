/**
 * @file input_manager.h
 * @brief Input handling system
 * 
 * Manages keyboard, mouse, and other input devices.
 */

#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <array>

class GLFWwindow;

/**
 * @enum Key
 * @brief Enumeration of important keys
 */
enum class Key : uint8_t {
    W, A, S, D,           // Movement
    SPACE, SHIFT,         // Actions
    E, Q,                 // UI/Block interaction
    ESC,                  // Menu
};

/**
 * @class InputManager
 * @brief Manages all input from keyboard and mouse
 */
class InputManager {
public:
    /**
     * @brief Constructor
     * @param window GLFW window pointer
     */
    InputManager(GLFWwindow* window);

    /**
     * @brief Destructor
     */
    ~InputManager();

    /**
     * @brief Update input state
     */
    void Update();

    /**
     * @brief Check if a key is currently pressed
     * @param key The key to check
     * @return true if the key is held down
     */
    bool IsKeyPressed(Key key) const;

    /**
     * @brief Check if a key was just pressed this frame
     * @param key The key to check
     * @return true if the key was just pressed
     */
    bool IsKeyJustPressed(Key key) const;

    /**
     * @brief Check if mouse button is pressed
     * @param button Mouse button (0 = left, 1 = right, 2 = middle)
     * @return true if the button is held down
     */
    bool IsMouseButtonPressed(uint32_t button) const;

    /**
     * @brief Check if mouse button was just pressed
     * @param button Mouse button (0 = left, 1 = right, 2 = middle)
     * @return true if the button was just pressed
     */
    bool IsMouseButtonJustPressed(uint32_t button) const;

    /**
     * @brief Get current mouse position
     * @return Mouse position in screen coordinates
     */
    glm::vec2 GetMousePosition() const { return mouse_position_; }

    /**
     * @brief Get mouse delta (movement since last frame)
     * @return Mouse movement delta
     */
    glm::vec2 GetMouseDelta() const { return mouse_delta_; }

    /**
     * @brief Set mouse position
     * @param x X coordinate
     * @param y Y coordinate
     */
    void SetMousePosition(double x, double y);

    /**
     * @brief Check if the window requested to close
     * @return true if close was requested
     */
    bool WindowCloseRequested() const;

    /**
     * @brief Center the mouse on the screen
     * @param window_width Width of the window
     * @param window_height Height of the window
     */
    void CenterMouse(uint32_t window_width, uint32_t window_height);

private:
    GLFWwindow* window_;
    std::array<bool, 9> current_keys_;
    std::array<bool, 9> previous_keys_;
    std::array<bool, 3> current_mouse_buttons_;
    std::array<bool, 3> previous_mouse_buttons_;
    glm::vec2 mouse_position_;
    glm::vec2 previous_mouse_position_;
    glm::vec2 mouse_delta_;
};
