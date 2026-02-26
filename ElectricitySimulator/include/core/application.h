/**
 * @file application.h
 * @brief Main application class for B-Lec
 * 
 * Manages the main game loop, system initialization, and overall control flow.
 */

#pragma once

#include <memory>
#include <cstdint>

class GLFWwindow;
class Renderer;
class World;
class Camera;
class InputManager;
class UISystem;
class BlockInteraction;
class BlockSelector;

/**
 * @class Application
 * @brief Core application class that manages the entire game lifecycle
 */
class Application {
public:
    /**
     * @brief Constructor
     */
    Application();

    /**
     * @brief Destructor
     */
    ~Application();

    /**
     * @brief Initialize the application
     * @return true if initialization was successful, false otherwise
     */
    bool Initialize();

    /**
     * @brief Run the main application loop
     */
    void Run();

    /**
     * @brief Shutdown the application and cleanup resources
     */
    void Shutdown();

    /**
     * @brief Check if the application is still running
     * @return true if the window is still open, false otherwise
     */
    bool IsRunning() const;

    /**
     * @brief Get the GLFW window pointer
     * @return Pointer to GLFWwindow
     */
    GLFWwindow* GetWindow() const { return window_; }

    /**
     * @brief Request the application to close
     */
    void RequestClose();

    /**
     * @brief Handle window resize events
     * @param width New window width
     * @param height New window height
     */
    void OnWindowResize(uint32_t width, uint32_t height);

private:
    /**
     * @brief Update game logic for the current frame
     * @param delta_time Time elapsed since last frame in seconds
     */
    void Update(float delta_time);

    /**
     * @brief Render the current frame
     */
    void Render();

    /**
     * @brief Handle input events
     */
    void ProcessInput();

private:
    GLFWwindow* window_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<World> world_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<InputManager> input_manager_;
    std::unique_ptr<UISystem> ui_system_;
    std::unique_ptr<BlockInteraction> block_interaction_;
    std::unique_ptr<BlockSelector> block_selector_;

    bool is_running_;
    uint32_t window_width_;
    uint32_t window_height_;
    const char* window_title_;

    // Timing
    double last_frame_time_;
};
