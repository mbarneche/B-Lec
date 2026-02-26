// renderer.cpp
// Implementation of basic rendering operations

#include "render/renderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void Renderer::SetProjection(const glm::mat4& projection) {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
    glMatrixMode(GL_MODELVIEW);
}

void Renderer::SetView(const glm::mat4& view) {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));
}

void Renderer::SetModel(const glm::mat4& model) {
    glMultMatrixf(glm::value_ptr(model));
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

void Renderer::Begin3D(int screenWidth, int screenHeight, float fovDegrees) {
    // Set up perspective projection
    float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    glm::mat4 projection = glm::perspective(glm::radians(fovDegrees), aspect, 0.1f, 100.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable depth testing for proper 3D rendering
    EnableDepthTest();

    // Assume view matrix will be set separately with SetView
}

void Renderer::End3D() {
    // Could restore state here if needed
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

void Renderer::EnableDepthTest() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Renderer::DisableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}

void Renderer::EnableBackfaceCulling() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Renderer::DisableBackfaceCulling() {
    glDisable(GL_CULL_FACE);
}

} // namespace render
} // namespace blec
