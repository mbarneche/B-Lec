// renderer.cpp
// Implementation of basic rendering operations

#include "render/renderer.h"
#include <GLFW/glfw3.h>

namespace blec {
namespace render {

void Renderer::Initialize() {
    // No special initialization needed for basic OpenGL 2.1
    // Future: could load shaders, setup buffers, etc.
}

void Renderer::SetViewport(int width, int height) {
    glViewport(0, 0, width, height);
}

void Renderer::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Begin2D(int screenWidth, int screenHeight) {
    // Save current matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Set up orthographic projection (top-left origin, Y points down)
    glOrtho(0.0, static_cast<double>(screenWidth),
            static_cast<double>(screenHeight), 0.0,
            -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing for 2D
    glDisable(GL_DEPTH_TEST);
}

void Renderer::End2D() {
    // Restore previous matrices
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void Renderer::DrawFilledRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void Renderer::SetColor(float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
}

void Renderer::EnableBlending() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::DisableBlending() {
    glDisable(GL_BLEND);
}

} // namespace render
} // namespace blec
