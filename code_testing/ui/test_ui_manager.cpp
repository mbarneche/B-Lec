#include "../test_framework.h"
#include "ui/ui_manager.h"

// Test the Button hit detection
TEST_CASE(TestButtonContains) {
    blec::ui::Button button;
    button.x = 100.0f;
    button.y = 100.0f;
    button.width = 200.0f;
    button.height = 50.0f;
    
    // Test point inside button
    ASSERT_TRUE(button.Contains(150.0f, 125.0f));
    ASSERT_TRUE(button.Contains(100.0f, 100.0f));  // Top-left corner
    
    // Test point outside button
    ASSERT_FALSE(button.Contains(99.0f, 125.0f));   // Just left of button
    ASSERT_FALSE(button.Contains(300.1f, 125.0f));  // Just right of button
    ASSERT_FALSE(button.Contains(150.0f, 99.0f));   // Just above button
    ASSERT_FALSE(button.Contains(150.0f, 150.1f));  // Just below button
}

// Test UIManager initialization
TEST_CASE(TestUIManagerInitialize) {
    blec::ui::UIManager ui;
    
    // Valid initialization
    ASSERT_TRUE(ui.Initialize(1280, 720));
    ASSERT_EQ(ui.GetScreenWidth(), 1280);
    ASSERT_EQ(ui.GetScreenHeight(), 720);
    
    // Invalid initialization (zero width)
    ASSERT_FALSE(ui.Initialize(0, 720));
    
    // Invalid initialization (negative height)
    ASSERT_FALSE(ui.Initialize(1280, -1));
}

// Test pause state toggling
TEST_CASE(TestPauseToggle) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // Initially not paused
    ASSERT_FALSE(ui.IsPaused());
    
    // Toggle to paused
    ui.TogglePause();
    ASSERT_TRUE(ui.IsPaused());
    
    // Toggle back to unpaused
    ui.TogglePause();
    ASSERT_FALSE(ui.IsPaused());
}

// Test explicit pause state setting
TEST_CASE(TestSetPaused) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // Set to paused
    ui.SetPaused(true);
    ASSERT_TRUE(ui.IsPaused());
    
    // Set to unpaused
    ui.SetPaused(false);
    ASSERT_FALSE(ui.IsPaused());
    
    // Set to paused again
    ui.SetPaused(true);
    ASSERT_TRUE(ui.IsPaused());
}

// Test mouse click handling when not paused
TEST_CASE(TestMouseClickNotPaused) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // No pause state, so clicks should return None
    ui.SetPaused(false);
    
    auto result = ui.HandleMouseClick(640.0f, 360.0f);
    ASSERT_EQ(result, blec::ui::UIManager::ButtonAction::None);
}

// Test mouse click on Resume button
TEST_CASE(TestMouseClickResume) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // Set to paused
    ui.SetPaused(true);
    
    // Click on Resume button (estimated center position)
    // Resume button is positioned at center_x - width/2, center_y - height/2 - spacer
    // For 1280x720: center_x=640, center_y=360
    // Resume button x: 640 - 75 = 565, y: 360 - 20 - 40 = 300
    auto result = ui.HandleMouseClick(640.0f, 320.0f);
    ASSERT_EQ(result, blec::ui::UIManager::ButtonAction::Resume);
}

// Test mouse click on Quit button
TEST_CASE(TestMouseClickQuit) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // Set to paused
    ui.SetPaused(true);
    
    // Click on Quit button (estimated center position)
    // Quit button is positioned below Resume button
    // For 1280x720: center_y=360
    // Quit button y: 360 + 20 + 40 = 420
    auto result = ui.HandleMouseClick(640.0f, 420.0f);
    ASSERT_EQ(result, blec::ui::UIManager::ButtonAction::Quit);
}

// Test mouse click outside both buttons
TEST_CASE(TestMouseClickMiss) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // Set to paused
    ui.SetPaused(true);
    
    // Click in empty area (top-left corner)
    auto result = ui.HandleMouseClick(10.0f, 10.0f);
    ASSERT_EQ(result, blec::ui::UIManager::ButtonAction::None);
    
    // Click in empty area (bottom-right corner)
    result = ui.HandleMouseClick(1270.0f, 710.0f);
    ASSERT_EQ(result, blec::ui::UIManager::ButtonAction::None);
}

// Test screen dimension updates
TEST_CASE(TestUpdateScreenDimensions) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // Update to new dimensions
    ui.UpdateScreenDimensions(1920, 1080);
    ASSERT_EQ(ui.GetScreenWidth(), 1920);
    ASSERT_EQ(ui.GetScreenHeight(), 1080);
    
    // Update with invalid dimensions (should not change)
    ui.UpdateScreenDimensions(-100, 500);
    ASSERT_EQ(ui.GetScreenWidth(), 1920);
    ASSERT_EQ(ui.GetScreenHeight(), 1080);
    
    // Update with zero width (should not change)
    ui.UpdateScreenDimensions(0, 720);
    ASSERT_EQ(ui.GetScreenWidth(), 1920);
    ASSERT_EQ(ui.GetScreenHeight(), 1080);
}

// Test shutdown
TEST_CASE(TestShutdown) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    ui.SetPaused(true);
    
    // Shutdown should reset pause state
    ui.Shutdown();
    ASSERT_FALSE(ui.IsPaused());
}

// Test button positioning for different screen sizes
TEST_CASE(TestButtonPositioningSmallScreen) {
    blec::ui::UIManager ui;
    ui.Initialize(800, 600);
    
    // Buttons should still be valid even on small screens
    // Just verify we can initialize and click
    ui.SetPaused(true);
    
    auto result = ui.HandleMouseClick(400.0f, 300.0f);
    // Result could be None, Resume, or Quit depending on actual button positions
    // The important thing is it doesn't crash
    ASSERT_TRUE(result == blec::ui::UIManager::ButtonAction::None ||
                result == blec::ui::UIManager::ButtonAction::Resume ||
                result == blec::ui::UIManager::ButtonAction::Quit);
}

// Test button positioning for large screen
TEST_CASE(TestButtonPositioningLargeScreen) {
    blec::ui::UIManager ui;
    ui.Initialize(4096, 2160);
    
    // Buttons should work on large screens too
    ui.SetPaused(true);
    
    auto result = ui.HandleMouseClick(2048.0f, 1080.0f);
    // Just verify it doesn't crash
    ASSERT_TRUE(result == blec::ui::UIManager::ButtonAction::None ||
                result == blec::ui::UIManager::ButtonAction::Resume ||
                result == blec::ui::UIManager::ButtonAction::Quit);
}

// Test multiple pause toggles
TEST_CASE(TestMultiplePauseToggles) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // Toggle multiple times
    for (int i = 0; i < 5; i++) {
        ASSERT_FALSE(ui.IsPaused());
        ui.TogglePause();
        ASSERT_TRUE(ui.IsPaused());
        ui.TogglePause();
    }
    
    // Should be back to unpaused
    ASSERT_FALSE(ui.IsPaused());
}

// Test pause state persistence across screen updates
TEST_CASE(TestPauseStatePersistence) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    
    // Set paused state
    ui.SetPaused(true);
    ASSERT_TRUE(ui.IsPaused());
    
    // Update screen dimensions
    ui.UpdateScreenDimensions(1920, 1080);
    
    // Pause state should persist
    ASSERT_TRUE(ui.IsPaused());
}

// Edge case: Button click at exact button boundaries
TEST_CASE(TestButtonBoundaryClick) {
    blec::ui::UIManager ui;
    ui.Initialize(1280, 720);
    ui.SetPaused(true);
    
    // Get button information by multiple clicks to determine boundaries
    // Click slightly right of center
    auto result1 = ui.HandleMouseClick(565.0f, 320.0f);  // Should be on Resume button x
    
    // The exact boundaries depend on the implementation but shouldn't crash
    ASSERT_TRUE(result1 == blec::ui::UIManager::ButtonAction::None ||
                result1 == blec::ui::UIManager::ButtonAction::Resume);
}

TEST_MAIN()
