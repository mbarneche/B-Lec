#include "ui/ui_manager.h"
#include "render/renderer.h"
#include "render/font.h"

namespace blec {
namespace ui {

UIManager::UIManager() = default;

UIManager::~UIManager() = default;

bool UIManager::Initialize(int screen_width, int screen_height) {
    // Validate input dimensions
    if (screen_width <= 0 || screen_height <= 0) {
        return false;
    }
    
    screen_width_ = screen_width;
    screen_height_ = screen_height;
    
    // Initialize pause state to false (not paused)
    is_paused_ = false;
    
    // Calculate button positions
    UpdateButtonPositions();
    
    return true;
}

void UIManager::Shutdown() {
    // No resources to clean up - this is a stateless UI manager
    // But keep method for API consistency
    is_paused_ = false;
}

void UIManager::TogglePause() {
    is_paused_ = !is_paused_;
}

void UIManager::SetPaused(bool paused) {
    is_paused_ = paused;
}

bool UIManager::IsPaused() const {
    return is_paused_;
}

void UIManager::UpdateButtonPositions() {
    // Center the menu buttons horizontally and position them in the middle of screen
    float menu_center_x = screen_width_ / 2.0f;
    float menu_center_y = screen_height_ / 2.0f;
    
    // Resume button (on top)
    resume_button_.x = menu_center_x - kButtonWidth / 2.0f;
    resume_button_.y = menu_center_y - kButtonHeight / 2.0f - (kButtonHeight + kButtonSpacing) / 2.0f;
    resume_button_.width = kButtonWidth;
    resume_button_.height = kButtonHeight;
    
    // Quit button (below resume button)
    quit_button_.x = menu_center_x - kButtonWidth / 2.0f;
    quit_button_.y = menu_center_y - kButtonHeight / 2.0f + (kButtonHeight + kButtonSpacing) / 2.0f;
    quit_button_.width = kButtonWidth;
    quit_button_.height = kButtonHeight;
}

UIManager::ButtonAction UIManager::HandleMouseClick(float mouse_x, float mouse_y) {
    // Only handle clicks when paused
    if (!is_paused_) {
        return ButtonAction::None;
    }
    
    // Check which button was clicked
    if (resume_button_.Contains(mouse_x, mouse_y)) {
        return ButtonAction::Resume;
    }
    if (quit_button_.Contains(mouse_x, mouse_y)) {
        return ButtonAction::Quit;
    }
    
    return ButtonAction::None;
}

void UIManager::RenderCrosshair(render::Renderer& renderer) const {
    float center_x = screen_width_ / 2.0f;
    float center_y = screen_height_ / 2.0f;
    
    // Set white color with transparency for crosshair
    renderer.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
    
    // Draw horizontal line of crosshair (using DrawLine)
    // Left part (gap in center)
    renderer.DrawLine(center_x - kCrosshairSize, center_y,
                      center_x - 5.0f, center_y);
    
    // Right part
    renderer.DrawLine(center_x + 5.0f, center_y,
                      center_x + kCrosshairSize, center_y);
    
    // Draw vertical line of crosshair
    // Top part
    renderer.DrawLine(center_x, center_y - kCrosshairSize,
                      center_x, center_y - 5.0f);
    
    // Bottom part
    renderer.DrawLine(center_x, center_y + 5.0f,
                      center_x, center_y + kCrosshairSize);
}

void UIManager::RenderPauseMenu(render::Renderer& renderer, render::BitmapFont& font) const {
    // Only render if paused
    if (!is_paused_) {
        return;
    }
    
    float center_x = screen_width_ / 2.0f;
    float center_y = screen_height_ / 2.0f;
    
    // Draw semi-transparent dark background overlay
    renderer.SetColor(0.0f, 0.0f, 0.0f, kMenuBackgroundAlpha);
    renderer.DrawFilledRect(0.0f, 0.0f,
                            static_cast<float>(screen_width_), 
                            static_cast<float>(screen_height_));
    
    // Draw "PAUSED" title at top of menu
    // Position it above the buttons
    float title_y = center_y - 80.0f;
    renderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);  // White text
    font.DrawText(center_x - 30.0f, title_y, 1.0f, "PAUSED");
    
    // Draw Resume button (green)
    renderer.SetColor(0.2f, 0.6f, 0.2f, 0.9f);
    renderer.DrawFilledRect(resume_button_.x, resume_button_.y,
                            resume_button_.width, resume_button_.height);
    
    // Draw Resume button text (centered in button)
    renderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);  // White text
    font.DrawText(resume_button_.x + 35.0f, resume_button_.y + 12.0f, 
                  1.0f, "Resume");
    
    // Draw Quit button (red)
    renderer.SetColor(0.6f, 0.2f, 0.2f, 0.9f);
    renderer.DrawFilledRect(quit_button_.x, quit_button_.y,
                            quit_button_.width, quit_button_.height);
    
    // Draw Quit button text (centered in button)
    renderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);  // White text
    font.DrawText(quit_button_.x + 48.0f, quit_button_.y + 12.0f, 
                  1.0f, "Quit");
}

void UIManager::UpdateScreenDimensions(int screen_width, int screen_height) {
    // Validate and update dimensions
    if (screen_width > 0 && screen_height > 0) {
        screen_width_ = screen_width;
        screen_height_ = screen_height;
        
        // Recalculate button positions for new screen size
        UpdateButtonPositions();
    }
}

int UIManager::GetScreenWidth() const {
    return screen_width_;
}

int UIManager::GetScreenHeight() const {
    return screen_height_;
}

} // namespace ui
} // namespace blec
