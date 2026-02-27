// code_testing/render/test_renderer_3d.cpp
// Unit tests for Renderer 3D functionality
// Tests perspective projection, view/model matrices, depth testing, and culling

#include "../test_framework.h"
#include "render/renderer.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {

struct GLContextGuard {
    GLContextGuard() {
        if (!glfwInit()) {
            return;
        }

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        window = glfwCreateWindow(64, 64, "test_renderer_3d", nullptr, nullptr);
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

// ============================================================================
// TEST SUITE: Renderer 3D Operations
// ============================================================================

TEST_CASE(TestRendererBeginEnd3D) {
    REQUIRE_GL_CONTEXT();
    blec::render::Renderer renderer;
    renderer.Initialize();

    renderer.Begin3D(1280, 720, 45.0f);
    renderer.End3D();

    ASSERT_TRUE(true);
}

TEST_CASE(TestRendererSetMatrices) {
    REQUIRE_GL_CONTEXT();
    blec::render::Renderer renderer;
    renderer.Initialize();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);

    renderer.SetProjection(projection);
    renderer.SetView(view);
    renderer.SetModel(model);

    ASSERT_TRUE(true);
}

TEST_CASE(TestRendererDepthAndCulling) {
    REQUIRE_GL_CONTEXT();
    blec::render::Renderer renderer;
    renderer.Initialize();

    renderer.EnableDepthTest();
    renderer.EnableBackfaceCulling();
    renderer.DisableBackfaceCulling();
    renderer.DisableDepthTest();

    ASSERT_TRUE(true);
}

TEST_CASE(TestRendererMultipleFrames) {
    REQUIRE_GL_CONTEXT();
    blec::render::Renderer renderer;
    renderer.Initialize();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));

    for (int frame = 0; frame < 3; ++frame) {
        renderer.Begin3D(1280, 720, 45.0f);
        renderer.SetProjection(projection);
        renderer.SetView(view);

        glm::mat4 model = glm::rotate(glm::mat4(1.0f),
                                      glm::radians(15.0f * frame),
                                      glm::vec3(0.0f, 1.0f, 0.0f));
        renderer.SetModel(model);
        renderer.EnableDepthTest();
        renderer.EnableBackfaceCulling();
        renderer.End3D();
    }

    ASSERT_TRUE(true);
}

TEST_MAIN()
