// test_input_handler.cpp
// Unit tests for InputHandler class
// Tests input tracking, key state, and mouse position

#include "test_framework.h"
#include "input/input_handler.h"
#include <GLFW/glfw3.h>

using namespace blec::input;

// Test InputHandler initialization
TEST_CASE(TestInputHandlerInit) {
    InputHandler input;
    
    // All keys should be released initially
    for (int key = 0; key < 348; ++key) {
        ASSERT_FALSE(input.IsKeyDown(key));
    }
    
    // Last key event should be empty
    ASSERT_TRUE(input.GetLastKeyEvent().empty());
}

// Test key name mapping
TEST_CASE(TestKeyNames) {
    // Test alphabetic keys
    ASSERT_STREQ(InputHandler::GetKeyName(GLFW_KEY_A).c_str(), "A");
    ASSERT_STREQ(InputHandler::GetKeyName(GLFW_KEY_Z).c_str(), "Z");
    
    // Test numeric keys
    ASSERT_STREQ(InputHandler::GetKeyName(GLFW_KEY_0).c_str(), "0");
    ASSERT_STREQ(InputHandler::GetKeyName(GLFW_KEY_9).c_str(), "9");
    
    // Test special keys
    ASSERT_STREQ(InputHandler::GetKeyName(GLFW_KEY_SPACE).c_str(), "Space");
    ASSERT_STREQ(InputHandler::GetKeyName(GLFW_KEY_ESCAPE).c_str(), "Esc");
    ASSERT_STREQ(InputHandler::GetKeyName(GLFW_KEY_F12).c_str(), "F12");
}

// Test action name mapping
TEST_CASE(TestActionNames) {
    ASSERT_STREQ(InputHandler::GetActionName(GLFW_PRESS), "press");
    ASSERT_STREQ(InputHandler::GetActionName(GLFW_RELEASE), "release");
    ASSERT_STREQ(InputHandler::GetActionName(GLFW_REPEAT), "repeat");
}

// Test key callback handling
TEST_CASE(TestKeyCallback) {
    InputHandler input;
    
    // Simulate key press
    input.OnKey(GLFW_KEY_W, 0, GLFW_PRESS, 0);
    ASSERT_TRUE(input.IsKeyDown(GLFW_KEY_W));
    ASSERT_FALSE(input.GetLastKeyEvent().empty());
    
    // Simulate key release
    input.OnKey(GLFW_KEY_W, 0, GLFW_RELEASE, 0);
    ASSERT_FALSE(input.IsKeyDown(GLFW_KEY_W));
}

// Test keys down string
TEST_CASE(TestKeysDownString) {
    InputHandler input;
    
    // No keys pressed - should return "(none)"
    std::string keys = input.GetKeysDownString();
    ASSERT_STREQ(keys.c_str(), "(none)");
    
    // Simulate pressing a key
    input.OnKey(GLFW_KEY_A, 0, GLFW_PRESS, 0);
    keys = input.GetKeysDownString();
    ASSERT_FALSE(keys.empty());
    ASSERT_NE(keys, "(none)");
}

// Test mouse position
TEST_CASE(TestMousePosition) {
    InputHandler input;
    
    double x = 0.0;
    double y = 0.0;
    input.GetMousePosition(&x, &y);
    
    // Initial position should be (0, 0)
    ASSERT_EQ(x, 0.0);
    ASSERT_EQ(y, 0.0);
    
    // Simulate cursor movement
    input.OnCursorPos(100.0, 200.0);
    input.GetMousePosition(&x, &y);
    ASSERT_EQ(x, 100.0);
    ASSERT_EQ(y, 200.0);
}

// Test mouse delta
TEST_CASE(TestMouseDelta) {
    InputHandler input;
    
    // First movement should have zero delta (no previous position)
    input.OnCursorPos(50.0, 50.0);
    double dx = 0.0;
    double dy = 0.0;
    input.GetMouseDelta(&dx, &dy);
    ASSERT_EQ(dx, 0.0);
    ASSERT_EQ(dy, 0.0);
    
    // Second movement should calculate delta
    input.OnCursorPos(60.0, 70.0);
    input.GetMouseDelta(&dx, &dy);
    ASSERT_EQ(dx, 10.0);
    ASSERT_EQ(dy, 20.0);
    
    // Reset delta
    input.ResetMouseDelta();
    input.GetMouseDelta(&dx, &dy);
    ASSERT_EQ(dx, 0.0);
    ASSERT_EQ(dy, 0.0);
}

TEST_MAIN()
