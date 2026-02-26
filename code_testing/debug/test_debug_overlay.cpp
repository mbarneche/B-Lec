// test_debug_overlay.cpp
// Unit tests for DebugOverlay class
// Tests debug state tracking and overlay visibility

#include "test_framework.h"
#include "debug/debug_overlay.h"
#include "input/input_handler.h"
#include "render/font.h"

using namespace blec::debug;
using namespace blec::input;
using namespace blec::render;

// Test debug overlay initialization
TEST_CASE(TestDebugOverlayInit) {
    DebugOverlay overlay;
    
    // Should start invisible
    ASSERT_FALSE(overlay.IsVisible());
    
    // FPS should start at 0
    ASSERT_EQ(overlay.GetFPS(), 0.0);
}

// Test overlay toggle
TEST_CASE(TestOverlayToggle) {
    DebugOverlay overlay;
    
    ASSERT_FALSE(overlay.IsVisible());
    
    overlay.Toggle();
    ASSERT_TRUE(overlay.IsVisible());
    
    overlay.Toggle();
    ASSERT_FALSE(overlay.IsVisible());
}

// Test explicit visibility setting
TEST_CASE(TestSetVisible) {
    DebugOverlay overlay;
    
    overlay.SetVisible(true);
    ASSERT_TRUE(overlay.IsVisible());
    
    overlay.SetVisible(false);
    ASSERT_FALSE(overlay.IsVisible());
    
    overlay.SetVisible(true);
    ASSERT_TRUE(overlay.IsVisible());
}

// Test FPS calculation update
TEST_CASE(TestFPSUpdate) {
    DebugOverlay overlay;
    InputHandler input;
    
    // Update with small delta times
    overlay.Update(input, 0.016);  // ~60 FPS
    overlay.Update(input, 0.016);
    overlay.Update(input, 0.016);
    
    // FPS should still be 0 until 1 second accumulates
    ASSERT_EQ(overlay.GetFPS(), 0.0);
}

// Test FPS calculation after 1 second
TEST_CASE(TestFPSCalculation) {
    DebugOverlay overlay;
    InputHandler input;
    
    // Simulate 60 frames at ~16.67ms each (1 second total)
    for (int i = 0; i < 60; ++i) {
        overlay.Update(input, 0.01667);
    }
    
    // FPS should be calculated now
    double fps = overlay.GetFPS();
    ASSERT_GT(fps, 0.0);
    ASSERT_LT(fps, 100.0);  // Should be reasonable
}

// Test error recording
TEST_CASE(TestErrorRecording) {
    DebugOverlay overlay;
    
    overlay.RecordError("Test error 1");
    overlay.RecordError("Test error 2");
    overlay.RecordError("Test error 3");
    
    // Errors are recorded (internal state)
    // We can't directly test the count without exposing it,
    // but we can verify no crashes occur
}

// Test warning recording
TEST_CASE(TestWarningRecording) {
    DebugOverlay overlay;
    
    overlay.RecordWarning("Test warning 1");
    overlay.RecordWarning("Test warning 2");
    
    // Same as errors - verify no crashes
}

// Test rendering when invisible
TEST_CASE(TestRenderInvisible) {
    DebugOverlay overlay;
    InputHandler input;
    BitmapFont font;
    
    overlay.SetVisible(false);
    
    // Rendering when invisible should be a no-op (no crash)
    overlay.Render(800, 600, font, input);
}

// Test rendering when visible (without OpenGL context)
TEST_CASE(TestRenderVisible) {
    DebugOverlay overlay;
    InputHandler input;
    BitmapFont font;
    
    overlay.SetVisible(true);
    
    // This will attempt to render but won't crash without GL context
    // (GL calls will be no-ops or fail gracefully)
    overlay.Render(800, 600, font, input);
}

// Test complete update and render cycle
TEST_CASE(TestUpdateAndRender) {
    DebugOverlay overlay;
    InputHandler input;
    BitmapFont font;
    
    overlay.SetVisible(true);
    
    // Simulate pressing a key
    input.OnKey(GLFW_KEY_W, 0, GLFW_PRESS, 0);
    
    // Update and render
    overlay.Update(input, 0.016);
    overlay.Render(800, 600, font, input);
    
    // Should complete without crashing
}

TEST_MAIN()
