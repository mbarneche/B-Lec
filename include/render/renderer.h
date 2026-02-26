// renderer.h
// Core rendering functionality using OpenGL
// Provides basic drawing primitives, 2D overlay support, and 3D rendering

#ifndef BLEC_RENDERER_H
#define BLEC_RENDERER_H

#include <glm/glm.hpp>

namespace blec {
namespace render {

// Renderer provides basic OpenGL rendering operations
// Manages matrix stacks, rendering state, and drawing primitives
class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    // Initialize renderer and set up OpenGL state
    void Initialize();

    // Set the OpenGL viewport to match framebuffer size
    static void SetViewport(int width, int height);

    // Clear the screen with a background color
    // Also clears depth buffer if depth testing is enabled
    static void Clear(float r, float g, float b, float a);

    // Set projection matrix (for perspective or orthographic projection)
    static void SetProjection(const glm::mat4& projection);

    // Set view matrix (camera transformation)
    static void SetView(const glm::mat4& view);

    // Set model matrix (object transformation)
    static void SetModel(const glm::mat4& model);

    // Begin drawing in 2D screen space (for overlays, UI)
    // Origin is top-left, Y axis points down
    static void Begin2D(int screenWidth, int screenHeight);

    // End 2D drawing and restore previous state
    static void End2D();

    // Begin drawing in 3D world space
    // Sets up perspective projection and enables depth testing
    static void Begin3D(int screenWidth, int screenHeight, float fovDegrees = 45.0f);

    // End 3D drawing and restore state
    static void End3D();

    // Draw a filled rectangle at (x, y) with size (w, h)
    // Used for 2D UI and overlays
    static void DrawFilledRect(float x, float y, float w, float h);

    // Set the current drawing color (RGBA, 0-1 range)
    static void SetColor(float r, float g, float b, float a);

    // Enable alpha blending for transparency
    static void EnableBlending();

    // Disable alpha blending
    static void DisableBlending();

    // Enable depth testing for 3D rendering
    // When enabled, closer objects are drawn in front of farther objects
    static void EnableDepthTest();

    // Disable depth testing
    static void DisableDepthTest();

    // Enable back-face culling for 3D rendering
    // When enabled, faces pointing away from camera aren't rendered
    static void EnableBackfaceCulling();

    // Disable back-face culling
    static void DisableBackfaceCulling();

private:
    // Non-copyable
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

} // namespace render
} // namespace blec

#endif // BLEC_RENDERER_H
