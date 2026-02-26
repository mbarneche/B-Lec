#ifndef BLEC_UI_UI_MANAGER_H
#define BLEC_UI_UI_MANAGER_H

#include <glm/glm.hpp>

namespace blec {
namespace render {
    class Renderer;
    class BitmapFont;
} // namespace render

namespace ui {

/// Represents a simple 2D button with position and dimensions
struct Button {
    float x;        ///< Left edge in screen coordinates (pixels)
    float y;        ///< Top edge in screen coordinates (pixels)
    float width;    ///< Button width in pixels
    float height;   ///< Button height in pixels
    
    /// Check if a point is inside this button
    /// @param px Point X coordinate
    /// @param py Point Y coordinate
    /// @return True if point is inside button bounds
    bool Contains(float px, float py) const {
        return px >= x && px < x + width &&
               py >= y && py < y + height;
    }
};

/// Manages UI elements including crosshair and pause menu
/// Handles rendering of UI elements and button interaction
class UIManager {
public:
    /// Enum for pause menu button actions
    enum class ButtonAction {
        None,       ///< No button pressed
        Resume,     ///< Resume button pressed
        Quit        ///< Quit button pressed
    };
    
    /// Constructor
    UIManager();
    
    /// Destructor
    ~UIManager();
    
    /// Initialize UI manager with screen dimensions
    /// @param screen_width Viewport width in pixels
    /// @param screen_height Viewport height in pixels
    /// @return True if initialization successful
    bool Initialize(int screen_width, int screen_height);
    
    /// Shutdown UI manager and clean up resources
    void Shutdown();
    
    /// Toggle pause state (pause <-> resume)
    void TogglePause();
    
    /// Set pause state explicitly
    /// @param paused True to pause, false to resume
    void SetPaused(bool paused);
    
    /// Check if game is currently paused
    /// @return True if paused, false if running
    bool IsPaused() const;
    
    /// Handle mouse click in pause menu
    /// @param mouse_x Mouse X position in screen coordinates (pixels)
    /// @param mouse_y Mouse Y position in screen coordinates (pixels)
    /// @return ButtonAction indicating which button was clicked (if any)
    ButtonAction HandleMouseClick(float mouse_x, float mouse_y);
    
    /// Render crosshair in center of screen
    /// Uses renderer's 2D drawing functions
    /// Should be called during 2D rendering phase
    void RenderCrosshair(render::Renderer& renderer) const;
    
    /// Render pause menu with semi-transparent background and buttons
    /// Uses renderer's 2D drawing functions
    /// Should be called during 2D rendering phase after other UI
    void RenderPauseMenu(render::Renderer& renderer, render::BitmapFont& font) const;
    
    /// Update screen dimensions (call if window resizes)
    /// @param screen_width New viewport width in pixels
    /// @param screen_height New viewport height in pixels
    void UpdateScreenDimensions(int screen_width, int screen_height);
    
    /// Get current screen width
    /// @return Screen width in pixels
    int GetScreenWidth() const;
    
    /// Get current screen height
    /// @return Screen height in pixels
    int GetScreenHeight() const;

private:
    /// Update button positions based on screen dimensions
    /// Called during initialization and screen size changes
    void UpdateButtonPositions();
    
    bool is_paused_ = false;            ///< Is game currently paused
    int screen_width_ = 0;              ///< Current screen width in pixels
    int screen_height_ = 0;             ///< Current screen height in pixels
    
    Button resume_button_;              ///< Resume button bounds
    Button quit_button_;                ///< Quit button bounds
    
    // Constants for UI layout
    static constexpr float kCrosshairSize = 20.0f;      ///< Crosshair line length in pixels
    static constexpr float kCrosshairThickness = 2.0f;  ///< Crosshair line thickness in pixels
    
    static constexpr float kMenuBackgroundAlpha = 0.7f;  ///< Background opacity (0-1)
    static constexpr float kButtonWidth = 150.0f;        ///< Button width in pixels
    static constexpr float kButtonHeight = 40.0f;        ///< Button height in pixels
    static constexpr float kButtonSpacing = 20.0f;       ///< Vertical spacing between buttons
};

} // namespace ui
} // namespace blec

#endif // BLEC_UI_UI_MANAGER_H
