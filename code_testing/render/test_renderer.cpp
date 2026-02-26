// test_renderer.cpp
// Unit tests for Renderer class
// Tests basic rendering operations (requires OpenGL context for full testing)

#include "test_framework.h"
#include "render/renderer.h"

using namespace blec::render;

// Test renderer initialization
TEST_CASE(TestRendererInit) {
    Renderer renderer;
    
    // Initialize should not crash
    renderer.Initialize();
}

// Test viewport setting
TEST_CASE(TestSetViewport) {
    // These calls should not crash (even without active GL context)
    Renderer::SetViewport(800, 600);
    Renderer::SetViewport(1920, 1080);
    Renderer::SetViewport(0, 0);
}

// Test clear color
TEST_CASE(TestClear) {
    // These calls should not crash
    Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);  // Black
    Renderer::Clear(1.0f, 1.0f, 1.0f, 1.0f);  // White
    Renderer::Clear(0.5f, 0.5f, 0.5f, 0.5f);  // Gray with alpha
}

// Test 2D rendering mode
TEST_CASE(Test2DMode) {
    // Begin2D and End2D should not crash
    Renderer::Begin2D(800, 600);
    Renderer::End2D();
    
    Renderer::Begin2D(1280, 720);
    Renderer::End2D();
}

// Test color setting
TEST_CASE(TestSetColor) {
    // SetColor should not crash with various values
    Renderer::SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    Renderer::SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    Renderer::SetColor(0.0f, 1.0f, 0.0f, 0.5f);
    Renderer::SetColor(0.5f, 0.5f, 1.0f, 0.25f);
}

// Test blending enable/disable
TEST_CASE(TestBlending) {
    // Blending operations should not crash
    Renderer::EnableBlending();
    Renderer::DisableBlending();
    
    // Multiple calls should be safe
    Renderer::EnableBlending();
    Renderer::EnableBlending();
    Renderer::DisableBlending();
    Renderer::DisableBlending();
}

// Test drawing primitives
TEST_CASE(TestDrawing) {
    // DrawFilledRect should not crash
    Renderer::DrawFilledRect(0.0f, 0.0f, 100.0f, 50.0f);
    Renderer::DrawFilledRect(10.0f, 20.0f, 0.0f, 0.0f);  // Zero size
    Renderer::DrawFilledRect(-10.0f, -10.0f, 20.0f, 20.0f);  // Negative position
}

// Test complete 2D rendering pipeline
TEST_CASE(TestCompletePipeline) {
    Renderer renderer;
    renderer.Initialize();
    
    // Complete rendering sequence should not crash
    Renderer::SetViewport(800, 600);
    Renderer::Clear(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::Begin2D(800, 600);
    Renderer::EnableBlending();
    Renderer::SetColor(1.0f, 0.0f, 0.0f, 0.5f);
    Renderer::DrawFilledRect(10.0f, 10.0f, 50.0f, 50.0f);
    Renderer::DisableBlending();
    Renderer::End2D();
}

TEST_MAIN()
