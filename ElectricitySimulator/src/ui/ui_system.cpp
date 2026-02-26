/**
 * @file ui_system.cpp
 * @brief UI system implementation
 */

#include "ui/ui_system.h"

UISystem::UISystem()
    : window_width_(1280), window_height_(720),
      show_debug_overlay_(false), show_block_palette_(true),
      camera_position_(0.0f, 0.0f, 0.0f), selected_block_("Air") {
}

UISystem::~UISystem() {
}

void UISystem::Initialize(uint32_t window_width, uint32_t window_height) {
    window_width_ = window_width;
    window_height_ = window_height;
}

void UISystem::Update(float delta_time) {
    // Update status messages
    for (auto it = status_messages_.begin(); it != status_messages_.end(); ) {
        it->elapsed += delta_time;
        if (it->elapsed >= it->duration) {
            it = status_messages_.erase(it);
        } else {
            ++it;
        }
    }
}

void UISystem::Render() {
    // Render debug overlay if enabled
    if (show_debug_overlay_) {
        RenderText("B-Lec Electricity Simulator", 10, 10, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText("Camera: " + std::to_string(camera_position_.x) + ", " +
                   std::to_string(camera_position_.y) + ", " +
                   std::to_string(camera_position_.z),
                   10, 30, 0.8f, glm::vec3(0.7f, 0.7f, 0.7f));
    }

    // Render block palette if enabled
    if (show_block_palette_) {
        RenderRect(10, window_height_ - 70, 200, 60, glm::vec3(0.2f, 0.2f, 0.2f), true);
        RenderText("Selected: " + selected_block_, 20, window_height_ - 50, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // Render status messages
    int32_t message_y = window_height_ - 100;
    for (const auto& msg : status_messages_) {
        float alpha = 1.0f - (msg.elapsed / msg.duration);
        RenderText(msg.text, 20, message_y, 0.9f, glm::vec3(1.0f, 1.0f, 1.0f) * alpha);
        message_y -= 20;
    }
}

void UISystem::SetCameraPosition(const glm::vec3& position) {
    camera_position_ = position;
}

void UISystem::SetSelectedBlock(const std::string& block_type) {
    selected_block_ = block_type;
}

void UISystem::AddStatusMessage(const std::string& message, float duration) {
    status_messages_.push_back({message, duration, 0.0f});
}

void UISystem::ToggleDebugOverlay() {
    show_debug_overlay_ = !show_debug_overlay_;
}

void UISystem::SetShowBlockPalette(bool show) {
    show_block_palette_ = show;
}

void UISystem::RenderText([[maybe_unused]] const std::string& text, [[maybe_unused]] int32_t x, [[maybe_unused]] int32_t y, [[maybe_unused]] float scale, [[maybe_unused]] const glm::vec3& color) {
    // Placeholder for text rendering
    // In a real implementation, this would use a font rendering library
}

void UISystem::RenderRect([[maybe_unused]] int32_t x, [[maybe_unused]] int32_t y, [[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t height, [[maybe_unused]] const glm::vec3& color, [[maybe_unused]] bool filled) {
    // Placeholder for rectangle rendering
    // In a real implementation, this would use OpenGL to draw rectangles
}
