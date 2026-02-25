/**
 * @file ui_system.h
 * @brief Simple UI system for HUD and menus
 * 
 * Manages on-screen UI elements like status information and menus.
 */

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

/**
 * @class UISystem
 * @brief Manages UI rendering and state
 */
class UISystem {
public:
    /**
     * @brief Constructor
     */
    UISystem();

    /**
     * @brief Destructor
     */
    ~UISystem();

    /**
     * @brief Initialize the UI system
     * @param window_width Width of the window
     * @param window_height Height of the window
     */
    void Initialize(uint32_t window_width, uint32_t window_height);

    /**
     * @brief Update UI state
     * @param delta_time Time since last frame
     */
    void Update(float delta_time);

    /**
     * @brief Render the UI
     */
    void Render();

    /**
     * @brief Set the camera position to display
     * @param position The camera position
     */
    void SetCameraPosition(const glm::vec3& position);

    /**
     * @brief Set the selected block type
     * @param block_type Name of the selected block type
     */
    void SetSelectedBlock(const std::string& block_type);

    /**
     * @brief Add a status message to display
     * @param message The message to display
     * @param duration How long to display the message in seconds
     */
    void AddStatusMessage(const std::string& message, float duration = 3.0f);

    /**
     * @brief Toggle the developer debug overlay
     */
    void ToggleDebugOverlay();

    /**
     * @brief Check if the debug overlay is enabled
     * @return true if debug overlay is visible
     */
    bool IsDebugOverlayEnabled() const { return show_debug_overlay_; }

    /**
     * @brief Set whether to show the block palette
     * @param show true to show the palette
     */
    void SetShowBlockPalette(bool show);

    /**
     * @brief Check if the block palette is shown
     * @return true if the palette is visible
     */
    bool IsBlockPaletteShown() const { return show_block_palette_; }

private:
    /**
     * @brief Render text to the screen
     * @param text The text to render
     * @param x Screen X position
     * @param y Screen Y position
     * @param scale Text scale factor
     * @param color Text color
     */
    void RenderText(const std::string& text, int32_t x, int32_t y, float scale, const glm::vec3& color);

    /**
     * @brief Render a rectangle
     * @param x X position
     * @param y Y position
     * @param width Width in pixels
     * @param height Height in pixels
     * @param color Color of the rectangle
     * @param filled Whether the rectangle should be filled
     */
    void RenderRect(int32_t x, int32_t y, uint32_t width, uint32_t height, const glm::vec3& color, bool filled);

private:
    uint32_t window_width_;
    uint32_t window_height_;
    bool show_debug_overlay_;
    bool show_block_palette_;
    glm::vec3 camera_position_;
    std::string selected_block_;
    
    struct StatusMessage {
        std::string text;
        float duration;
        float elapsed;
    };
    std::vector<StatusMessage> status_messages_;
};
