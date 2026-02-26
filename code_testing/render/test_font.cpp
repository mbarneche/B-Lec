// test_font.cpp
// Unit tests for BitmapFont class
// Tests character rendering dimensions and text operations

#include "test_framework.h"
#include "render/font.h"

using namespace blec::render;

// Test font character dimensions
TEST_CASE(TestFontDimensions) {
    BitmapFont font;
    
    float scale = 1.0f;
    ASSERT_EQ(font.GetCharWidth(scale), 6.0f);
    ASSERT_EQ(font.GetCharHeight(scale), 8.0f);
    
    scale = 2.0f;
    ASSERT_EQ(font.GetCharWidth(scale), 12.0f);
    ASSERT_EQ(font.GetCharHeight(scale), 16.0f);
    
    scale = 0.5f;
    ASSERT_EQ(font.GetCharWidth(scale), 3.0f);
    ASSERT_EQ(font.GetCharHeight(scale), 4.0f);
}

// Test that DrawChar doesn't crash with valid characters
// Note: Actual rendering requires OpenGL context, so we just test it doesn't crash
TEST_CASE(TestDrawCharValidRange) {
    BitmapFont font;
    
    // These calls should not crash (even without GL context, they'll just be no-ops)
    // Testing printable ASCII range
    for (char c = 32; c <= 126; ++c) {
        font.DrawChar(0.0f, 0.0f, 1.0f, c);
    }
}

// Test that DrawChar handles invalid characters gracefully
TEST_CASE(TestDrawCharInvalidRange) {
    BitmapFont font;
    
    // These should be safely ignored
    font.DrawChar(0.0f, 0.0f, 1.0f, '\0');   // Null
    font.DrawChar(0.0f, 0.0f, 1.0f, '\n');   // Newline (below printable range)
    font.DrawChar(0.0f, 0.0f, 1.0f, '\t');   // Tab
    font.DrawChar(0.0f, 0.0f, 1.0f, 127);    // DEL (above printable range)
}

// Test DrawText with various strings
TEST_CASE(TestDrawText) {
    BitmapFont font;
    
    // These should not crash
    font.DrawText(0.0f, 0.0f, 1.0f, "Hello, World!");
    font.DrawText(0.0f, 0.0f, 2.0f, "");  // Empty string
    font.DrawText(10.0f, 20.0f, 1.5f, "Line1\nLine2\nLine3");  // Multi-line
    font.DrawText(0.0f, 0.0f, 1.0f, "123 456 789");  // Numbers and spaces
}

// Test multi-line text handling
TEST_CASE(TestMultilineText) {
    BitmapFont font;
    
    // Multi-line text with newlines should not crash
    std::string multiline = "First line\nSecond line\nThird line";
    font.DrawText(0.0f, 0.0f, 1.0f, multiline);
}

TEST_MAIN()
