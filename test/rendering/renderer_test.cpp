#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/rendering/renderer.h"
#include "../../ElectricitySimulator/include/world/world.h"
#include "../../ElectricitySimulator/include/camera/camera.h"
#include "../test_helpers.h"

TEST(RendererTest, InitializeAndRender) {
    GlfwTestContext context;
    glfwMakeContextCurrent(context.window());

    Renderer renderer;
    ASSERT_TRUE(renderer.Initialize(800, 600));

    World world;
    world.Initialize();

    Camera camera(glm::vec3(0.0f, 5.0f, 5.0f),
                  glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));
    camera.SetAspectRatio(800.0f / 600.0f);

    renderer.SetViewport(0, 0, 800, 600);
    renderer.Clear(0.2f, 0.3f, 0.4f, 1.0f);
    renderer.Render(&world, &camera);
    renderer.RenderCube(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}
