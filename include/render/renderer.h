// renderer.h
// Core rendering functionality using OpenGL
// Provides basic drawing primitives and rendering state management

#ifndef BLEC_RENDERER_H
#define BLEC_RENDERER_H

namespace blec {
namespace render {

// Renderer provides basic OpenGL rendering operations
// Manages rendering state and provides drawing primitives
class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    // Initialize renderer and set up OpenGL state
    void Initialize();

    // Set the OpenGL viewport to match framebuffer size
    static void SetViewport(int width, int height);

    // Clear the screen with a background color
    static void Clear(float r, float g, float b, float a);

    // Begin drawing in 2D screen space (for overlays, UI)
    // Origin is top-left, Y axis points down
    static void Begin2D(int screenWidth, int screenHeight);

    // End 2D drawing and restore previous state
    static void End2D();

    // Draw a filled rectangle at (x, y) with size (w, h)
    static void DrawFilledRect(float x, float y, float w, float h);

    // Set the current drawing color (RGBA, 0-1 range)
    static void SetColor(float r, float g, float b, float a);

    // Enable alpha blending for transparency
    static void EnableBlending();

    // Disable alpha blending
    static void DisableBlending();

private:
    // Non-copyable
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

} // namespace render
} // namespace blec

#endif // BLEC_RENDERER_H
