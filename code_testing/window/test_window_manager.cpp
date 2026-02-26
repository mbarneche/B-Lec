// test_window_manager.cpp
// Unit tests for WindowManager class
// Tests window creation, lifecycle, and basic operations

#include "test_framework.h"
#include "window/window_manager.h"
#include <GLFW/glfw3.h>

using namespace blec::window;

// Test WindowManager initialization
TEST_CASE(TestWindowManagerInit) {
    WindowManager wm;
    
    // GLFW initialization should succeed
    ASSERT_TRUE(wm.InitializeGLFW());
    
    // Calling InitializeGLFW again should still succeed
    ASSERT_TRUE(wm.InitializeGLFW());
    
    wm.Shutdown();
}

// Test window creation
TEST_CASE(TestWindowCreation) {
    WindowManager wm;
    
    ASSERT_TRUE(wm.InitializeGLFW());
    ASSERT_TRUE(wm.CreateWindow(800, 600, "Test Window"));
    
    // GetHandle should return a valid window
    ASSERT_NOT_NULL(wm.GetHandle());
    
    // Window should not be marked to close initially
    ASSERT_FALSE(wm.ShouldClose());
    
    wm.Shutdown();
}

// Test window closing
TEST_CASE(TestWindowClosing) {
    WindowManager wm;
    
    ASSERT_TRUE(wm.InitializeGLFW());
    ASSERT_TRUE(wm.CreateWindow(800, 600, "Test Window"));
    
    // Set window to close
    wm.SetShouldClose(true);
    ASSERT_TRUE(wm.ShouldClose());
    
    // Set window to not close
    wm.SetShouldClose(false);
    ASSERT_FALSE(wm.ShouldClose());
    
    wm.Shutdown();
}

// Test framebuffer size
TEST_CASE(TestFramebufferSize) {
    WindowManager wm;
    
    ASSERT_TRUE(wm.InitializeGLFW());
    ASSERT_TRUE(wm.CreateWindow(640, 480, "Test Window"));
    wm.MakeContextCurrent();
    
    int width = 0;
    int height = 0;
    wm.GetFramebufferSize(&width, &height);
    
    // Framebuffer size should be positive
    ASSERT_GT(width, 0);
    ASSERT_GT(height, 0);
    
    wm.Shutdown();
}

// Test multiple shutdown calls (should be safe)
TEST_CASE(TestMultipleShutdown) {
    WindowManager wm;
    
    ASSERT_TRUE(wm.InitializeGLFW());
    ASSERT_TRUE(wm.CreateWindow(800, 600, "Test Window"));
    
    // Multiple shutdown calls should be safe
    wm.Shutdown();
    wm.Shutdown();
    wm.Shutdown();
}

TEST_MAIN()
