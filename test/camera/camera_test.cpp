#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/camera/camera.h"
#include <cmath>

TEST(CameraTest, MovementAndRotation) {
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 forward = camera.GetForward();
    EXPECT_NEAR(forward.x, 0.0f, 1e-5f);
    EXPECT_NEAR(forward.y, 0.0f, 1e-5f);
    EXPECT_NEAR(forward.z, -1.0f, 1e-5f);

    camera.MoveForward(2.0f);
    EXPECT_NEAR(camera.GetPosition().z, -2.0f, 1e-5f);

    camera.MoveRight(1.0f);
    EXPECT_NEAR(camera.GetPosition().x, 1.0f, 1e-5f);

    camera.MoveUp(3.0f);
    EXPECT_NEAR(camera.GetPosition().y, 3.0f, 1e-5f);

    camera.SetAspectRatio(2.0f);
    EXPECT_FLOAT_EQ(camera.GetAspectRatio(), 2.0f);

    camera.Rotate(90.0f, 100.0f);
    glm::vec3 new_forward = camera.GetForward();
    EXPECT_NEAR(new_forward.y, std::sin(glm::radians(89.0f)), 1e-3f);

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 proj = camera.GetProjectionMatrix();
    EXPECT_NEAR(view[3][3], 1.0f, 1e-5f);
    EXPECT_NEAR(proj[3][3], 0.0f, 1e-5f);
}
