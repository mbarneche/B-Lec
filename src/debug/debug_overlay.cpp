// debug_overlay.cpp
// Implementation of debug overlay rendering and state tracking

#include "debug/debug_overlay.h"
#include "input/input_handler.h"
#include "render/font.h"
#include "render/renderer.h"

#include <cstdio>

namespace blec {
namespace debug {

DebugOverlay::DebugOverlay()
    : visible_(false)
    , fps_(0.0)
    , frame_accumulator_(0.0)
    , frame_count_(0)
    , camera_x_(0.0f)
    , camera_y_(0.0f)
    , camera_z_(0.0f)
    , camera_yaw_(0.0f)
    , camera_pitch_(0.0f)
    , total_blocks_(0)
    , visible_blocks_(0)
    , error_count_(0)
    , last_error_()
    , warning_count_(0)
    , last_warning_() {
}

void DebugOverlay::Toggle() {
    visible_ = !visible_;
}

void DebugOverlay::Update(const input::InputHandler& input, double deltaTime) {
    (void)input;  // Input is queried during Render/BuildDebugLines.

    // Update FPS calculation
    frame_accumulator_ += deltaTime;
    frame_count_ += 1;

    // Update FPS every second
    if (frame_accumulator_ >= 1.0) {
        fps_ = static_cast<double>(frame_count_) / frame_accumulator_;
        frame_count_ = 0;
        frame_accumulator_ = 0.0;
    }
}

void DebugOverlay::Render(int screenWidth, int screenHeight, const render::BitmapFont& font, const input::InputHandler& input) const {
    if (!visible_) {
        return;
    }

    // Build debug information lines
    std::vector<std::string> lines = BuildDebugLines(input);

    // Calculate overlay dimensions
    const float scale = 2.0f;
    const float padding = 10.0f;
    const float lineHeight = font.GetCharHeight(scale);

    float maxWidth = 0.0f;
    for (const auto& line : lines) {
        float width = static_cast<float>(line.size()) * font.GetCharWidth(scale);
        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    float boxWidth = maxWidth + padding * 2.0f;
    float boxHeight = lineHeight * static_cast<float>(lines.size()) + padding * 2.0f;

    // Begin 2D rendering
    render::Renderer::Begin2D(screenWidth, screenHeight);
    render::Renderer::EnableBlending();

    // Draw semi-transparent background box
    render::Renderer::SetColor(0.0f, 0.0f, 0.0f, 0.6f);
    render::Renderer::DrawFilledRect(padding, padding, boxWidth, boxHeight);

    // Draw debug text
    render::Renderer::SetColor(0.9f, 0.95f, 1.0f, 1.0f);
    float textY = padding * 2.0f;
    for (const auto& line : lines) {
        font.DrawText(padding * 2.0f, textY, scale, line);
        textY += lineHeight;
    }

    render::Renderer::DisableBlending();
    render::Renderer::End2D();
}

void DebugOverlay::RecordError(const std::string& error) {
    error_count_ += 1;
    last_error_ = error;
}

void DebugOverlay::RecordWarning(const std::string& warning) {
    warning_count_ += 1;
    last_warning_ = warning;
}

void DebugOverlay::SetCameraPosition(float x, float y, float z) {
    camera_x_ = x;
    camera_y_ = y;
    camera_z_ = z;
}

void DebugOverlay::SetCameraOrientation(float yaw, float pitch) {
    camera_yaw_ = yaw;
    camera_pitch_ = pitch;
}

void DebugOverlay::SetBlockCounts(uint32_t total_blocks, uint32_t visible_blocks) {
    total_blocks_ = total_blocks;
    visible_blocks_ = visible_blocks;
}

std::vector<std::string> DebugOverlay::BuildDebugLines(const input::InputHandler& input) const {
    char buffer[256];
    std::vector<std::string> lines;

    // ======== Game State Section ========
    lines.emplace_back("=== GAME STATE ===");

    // FPS
    std::snprintf(buffer, sizeof(buffer), "FPS: %.1f", fps_);
    lines.emplace_back(buffer);

    // ======== Camera Section ========
    lines.emplace_back("=== CAMERA ===");

    // Camera position
    std::snprintf(buffer, sizeof(buffer), "Position: (%.2f, %.2f, %.2f)", camera_x_, camera_y_, camera_z_);
    lines.emplace_back(buffer);

    // Camera orientation (convert radians to degrees for display)
    float yaw_deg = camera_yaw_ * 180.0f / 3.14159265f;
    float pitch_deg = camera_pitch_ * 180.0f / 3.14159265f;
    std::snprintf(buffer, sizeof(buffer), "Rotation: Yaw=%.1f° Pitch=%.1f°", yaw_deg, pitch_deg);
    lines.emplace_back(buffer);

    // ======== Block System Section ========
    lines.emplace_back("=== BLOCKS ===");

    // Block counts
    std::snprintf(buffer, sizeof(buffer), "Total Blocks: %u", total_blocks_);
    lines.emplace_back(buffer);

    std::snprintf(buffer, sizeof(buffer), "Visible Blocks: %u", visible_blocks_);
    lines.emplace_back(buffer);

    // ======== Input Section ========
    lines.emplace_back("=== INPUT ===");

    // Keys currently pressed
    lines.emplace_back("Keys down: " + input.GetKeysDownString());

    // Last key event
    const std::string& lastKey = input.GetLastKeyEvent();
    lines.emplace_back("Last key: " + (lastKey.empty() ? "(none)" : lastKey));

    // Mouse position and delta
    double mouseX, mouseY, mouseDx, mouseDy;
    input.GetMousePosition(&mouseX, &mouseY);
    input.GetMouseDelta(&mouseDx, &mouseDy);

    std::snprintf(buffer, sizeof(buffer), "Mouse: x=%.0f y=%.0f dx=%.1f dy=%.1f",
                  mouseX, mouseY, mouseDx, mouseDy);
    lines.emplace_back(buffer);

    // ======== Error/Warning Section ========
    if (error_count_ > 0 || warning_count_ > 0) {
        lines.emplace_back("=== ISSUES ===");

        // Error count
        std::snprintf(buffer, sizeof(buffer), "Errors: %d", error_count_);
        lines.emplace_back(buffer);

        if (!last_error_.empty()) {
            lines.emplace_back("Last error: " + last_error_);
        }

        // Warning count
        std::snprintf(buffer, sizeof(buffer), "Warnings: %d", warning_count_);
        lines.emplace_back(buffer);

        if (!last_warning_.empty()) {
            lines.emplace_back("Last warning: " + last_warning_);
        }
    }

    return lines;
}

} // namespace debug
} // namespace blec
