// test_renderer.cpp
// Unit tests for Renderer class
// Tests basic rendering operations (requires OpenGL context for full testing)

#include "test_framework.h"
#include "render/renderer.h"
#include <GLFW/glfw3.h>

namespace {

struct GLContextGuard {
    GLContextGuard() {
        if (!glfwInit()) {
            return;
        }

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        window = glfwCreateWindow(64, 64, "test_renderer", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
        ok = true;
    }

    ~GLContextGuard() {
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    GLFWwindow* window = nullptr;
    bool ok = false;
};

const GLContextGuard g_gl_context;

bool HasGLContext() {
    return g_gl_context.ok;
}

#define REQUIRE_GL_CONTEXT() \
    if (!HasGLContext()) { \
        return; \
    }

} // namespace

using namespace blec::render;

// Test renderer initialization
TEST_CASE(TestRendererInit) {
    REQUIRE_GL_CONTEXT();
    Renderer renderer;
    
    // Initialize should not crash
    renderer.Initialize();
}

// Test viewport setting
TEST_CASE(TestSetViewport) {
    REQUIRE_GL_CONTEXT();
    // These calls should not crash (even without active GL context)
    Renderer::SetViewport(800, 600);
    Renderer::SetViewport(1920, 1080);
    Renderer::SetViewport(0, 0);
}

// Test clear color
TEST_CASE(TestClear) {
    REQUIRE_GL_CONTEXT();
    // These calls should not crash
    Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);  // Black
    Renderer::Clear(1.0f, 1.0f, 1.0f, 1.0f);  // White
    Renderer::Clear(0.5f, 0.5f, 0.5f, 0.5f);  // Gray with alpha
}

// Test 2D rendering mode
TEST_CASE(Test2DMode) {
    REQUIRE_GL_CONTEXT();
    // Begin2D and End2D should not crash
    Renderer::Begin2D(800, 600);
    Renderer::End2D();
    
    Renderer::Begin2D(1280, 720);
    Renderer::End2D();
}

// Test color setting
TEST_CASE(TestSetColor) {
    REQUIRE_GL_CONTEXT();
    // SetColor should not crash with various values
    Renderer::SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    Renderer::SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    Renderer::SetColor(0.0f, 1.0f, 0.0f, 0.5f);
    Renderer::SetColor(0.5f, 0.5f, 1.0f, 0.25f);
}

// Test blending enable/disable
TEST_CASE(TestBlending) {
    REQUIRE_GL_CONTEXT();
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
    REQUIRE_GL_CONTEXT();
    // DrawFilledRect should not crash
    Renderer::DrawFilledRect(0.0f, 0.0f, 100.0f, 50.0f);
    Renderer::DrawFilledRect(10.0f, 20.0f, 0.0f, 0.0f);  // Zero size
    Renderer::DrawFilledRect(-10.0f, -10.0f, 20.0f, 20.0f);  // Negative position
}

// Test complete 2D rendering pipeline
TEST_CASE(TestCompletePipeline) {
    REQUIRE_GL_CONTEXT();
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
