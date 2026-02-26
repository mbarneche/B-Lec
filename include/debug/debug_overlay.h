// debug_overlay.h
// Debug information overlay displaying FPS, input state, and errors
// Toggleable with F12 key

#ifndef BLEC_DEBUG_OVERLAY_H
#define BLEC_DEBUG_OVERLAY_H

#include <string>
#include <vector>

namespace blec {
namespace input {
    class InputHandler;
}
namespace render {
    class BitmapFont;
}

namespace debug {

// DebugOverlay displays real-time debug information on screen
// Shows FPS, input state, mouse position, and error/warning counts
class DebugOverlay {
public:
    DebugOverlay();
    ~DebugOverlay() = default;

    // Toggle overlay visibility
    void Toggle();

    // Set overlay visibility explicitly
    void SetVisible(bool visible) { visible_ = visible; }

    // Check if overlay is visible
    bool IsVisible() const { return visible_; }

    // Update debug information (call once per frame)
    void Update(const input::InputHandler& input, double deltaTime);

    // Render the overlay to screen
    void Render(int screenWidth, int screenHeight, const render::BitmapFont& font, const input::InputHandler& input) const;

    // Record an error
    void RecordError(const std::string& error);

    // Record a warning
    void RecordWarning(const std::string& warning);

    // Get current FPS
    double GetFPS() const { return fps_; }

private:
    // Build text lines for display
    std::vector<std::string> BuildDebugLines(const input::InputHandler& input) const;

    // Internal state
    bool visible_;
    double fps_;
    double frame_accumulator_;
    int frame_count_;

    // Error and warning tracking
    int error_count_;
    std::string last_error_;
    int warning_count_;
    std::string last_warning_;

    // Non-copyable
    DebugOverlay(const DebugOverlay&) = delete;
    DebugOverlay& operator=(const DebugOverlay&) = delete;
};

} // namespace debug
} // namespace blec

#endif // BLEC_DEBUG_OVERLAY_H
