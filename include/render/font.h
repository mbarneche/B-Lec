// font.h
// Bitmap font rendering for debug text display
// Uses 5x7 pixel font for ASCII characters 32-126

#ifndef BLEC_FONT_H
#define BLEC_FONT_H

#include <string>

namespace blec {
namespace render {

// BitmapFont provides simple text rendering using a 5x7 pixel font
// Public domain style bitmap font for ASCII printable characters
class BitmapFont {
public:
    BitmapFont() = default;
    ~BitmapFont() = default;

    // Draw a single character at screen position (x, y) with given scale
    // Characters outside ASCII 32-126 are ignored
    void DrawChar(float x, float y, float scale, char c) const;

    // Draw a text string at screen position (x, y) with given scale
    // Supports newline characters (\n) for multi-line text
    void DrawText(float x, float y, float scale, const std::string& text) const;

    // Get the width of a single character in pixels
    static constexpr float GetCharWidth(float scale) { return 6.0f * scale; }

    // Get the height of a single character in pixels
    static constexpr float GetCharHeight(float scale) { return 8.0f * scale; }

private:
    // 5x7 bitmap font data for ASCII 32-126 (95 characters)
    // Each character is 5 bytes, each byte is a column of 7 pixels
    static const unsigned char kFont5x7[96][5];
};

} // namespace render
} // namespace blec

#endif // BLEC_FONT_H
