// code_testing/render/test_camera.cpp
// Unit tests for Camera class
// Tests free-flying camera with WASD movement and mouse look rotation

#include "../test_framework.h"
#include "render/camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// ============================================================================
// TEST SUITE: Camera Initialization and Basic Properties
// ============================================================================

TEST_CASE(TestCameraInitialization) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    glm::vec3 pos = camera.GetPosition();
    ASSERT_TRUE(pos == glm::vec3(0.0f, 0.0f, 5.0f));

    glm::vec3 forward = camera.GetForward();
    ASSERT_TRUE(std::abs(forward.z) > 0.9f);
    ASSERT_TRUE(std::abs(forward.y) < 0.1f);
}

// ============================================================================
// TEST SUITE: Camera Movement
// ============================================================================

TEST_CASE(TestCameraMoveForward) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));

    camera.MoveForward(1.0f);
    camera.Update(0.016f);

    glm::vec3 new_pos = camera.GetPosition();
    ASSERT_TRUE(std::abs(new_pos.x - start_pos.x) < 0.01f);
    ASSERT_TRUE(std::abs(new_pos.y - start_pos.y) < 0.01f);
    ASSERT_TRUE(new_pos.z < start_pos.z);
}

TEST_CASE(TestCameraMoveBackward) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));

    camera.MoveForward(-1.0f);
    camera.Update(0.016f);

    glm::vec3 new_pos = camera.GetPosition();
    ASSERT_TRUE(new_pos.z > start_pos.z);
}

TEST_CASE(TestCameraMoveRight) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));

    camera.MoveRight(1.0f);
    camera.Update(0.016f);

    glm::vec3 new_pos = camera.GetPosition();
    ASSERT_TRUE(new_pos.x > start_pos.x);
    ASSERT_TRUE(std::abs(new_pos.y - start_pos.y) < 0.01f);
    ASSERT_TRUE(std::abs(new_pos.z - start_pos.z) < 0.1f);
}

TEST_CASE(TestCameraMoveUp) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));

    camera.MoveUp(1.0f);
    camera.Update(0.016f);

    glm::vec3 new_pos = camera.GetPosition();
    ASSERT_TRUE(new_pos.y > start_pos.y);
    ASSERT_TRUE(std::abs(new_pos.x - start_pos.x) < 0.01f);
    ASSERT_TRUE(std::abs(new_pos.z - start_pos.z) < 0.1f);
}

// ============================================================================
// TEST SUITE: Camera Rotation
// ============================================================================

TEST_CASE(TestCameraYaw) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f));

    glm::vec3 forward_before = camera.GetForward();
    camera.Yaw(20.0f);
    camera.Update(0.016f);
    glm::vec3 forward_after = camera.GetForward();

    ASSERT_TRUE(glm::length(forward_after - forward_before) > 0.001f);
}

TEST_CASE(TestCameraPitchClamping) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f));

    for (int i = 0; i < 200; ++i) {
        camera.Pitch(20.0f);
    }
    camera.Update(0.016f);

    glm::vec3 forward = camera.GetForward();
    ASSERT_TRUE(std::abs(forward.y) < 0.999f);
}

TEST_CASE(TestRotationSpeedAffectsRotation) {
    blec::render::Camera camera_fast;
    blec::render::Camera camera_slow;

    camera_fast.Initialize(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    camera_slow.Initialize(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    camera_fast.SetRotationSpeed(0.02f);
    camera_slow.SetRotationSpeed(0.005f);

    camera_fast.Yaw(10.0f);
    camera_slow.Yaw(10.0f);
    camera_fast.Update(0.016f);
    camera_slow.Update(0.016f);

    float delta = glm::length(camera_fast.GetForward() - camera_slow.GetForward());
    ASSERT_TRUE(delta > 0.01f);
}

// ============================================================================
// TEST SUITE: Movement Speed
// ============================================================================

TEST_CASE(TestMovementSpeedAffectsDistance) {
    blec::render::Camera camera_fast;
    blec::render::Camera camera_slow;
    glm::vec3 start(0.0f, 0.0f, 0.0f);

    camera_fast.Initialize(start, glm::vec3(0.0f, 0.0f, -5.0f));
    camera_slow.Initialize(start, glm::vec3(0.0f, 0.0f, -5.0f));

    camera_fast.SetMovementSpeed(10.0f);
    camera_slow.SetMovementSpeed(5.0f);

    camera_fast.MoveForward(1.0f);
    camera_slow.MoveForward(1.0f);

    camera_fast.Update(0.016f);
    camera_slow.Update(0.016f);

    float dist_fast = glm::length(camera_fast.GetPosition() - start);
    float dist_slow = glm::length(camera_slow.GetPosition() - start);
    ASSERT_TRUE(dist_fast > dist_slow);
}

// ============================================================================
// TEST SUITE: View Matrix
// ============================================================================

TEST_CASE(TestCameraViewMatrixOrthonormality) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f));
    camera.Update(0.016f);

    glm::vec3 forward = camera.GetForward();
    glm::vec3 right = camera.GetRight();
    glm::vec3 up = camera.GetUp();

    ASSERT_TRUE(std::abs(glm::length(forward) - 1.0f) < 0.01f);
    ASSERT_TRUE(std::abs(glm::length(right) - 1.0f) < 0.01f);
    ASSERT_TRUE(std::abs(glm::length(up) - 1.0f) < 0.01f);

    ASSERT_TRUE(std::abs(glm::dot(forward, right)) < 0.01f);
    ASSERT_TRUE(std::abs(glm::dot(forward, up)) < 0.01f);
    ASSERT_TRUE(std::abs(glm::dot(right, up)) < 0.01f);
}

TEST_MAIN()
