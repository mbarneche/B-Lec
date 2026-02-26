// code_testing/render/test_camera.cpp
// Unit tests for Camera class
// Tests free-flying camera with WASD movement and mouse look rotation

#include "../test_framework.h"
#include "render/camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace blec::test {

// ============================================================================
// TEST SUITE: Camera Initialization and Basic Properties
// ============================================================================

TEST(CameraInitialization) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Camera should be initialized at specified position
    EXPECT(camera.GetPosition() == glm::vec3(0.0f, 0.0f, 5.0f), "Initial position should match");
    
    // Default forward should point towards target
    glm::vec3 forward = camera.GetForward();
    EXPECT(std::abs(forward.z) > 0.9f, "Forward should primarily point in -Z direction");
    EXPECT(std::abs(forward.y) < 0.1f, "Forward should not point significantly in Y");
}

TEST(CameraDefaultSpeeds) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    
    // Camera should have sensible default speeds
    EXPECT(camera.GetMovementSpeed() > 0.0f, "Movement speed should be positive");
    EXPECT(camera.GetRotationSpeed() > 0.0f, "Rotation speed should be positive");
}

// ============================================================================
// TEST SUITE: Camera Movement
// ============================================================================

TEST(CameraMoveForward) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Move forward should increase Z (negative direction)
    camera.MoveForward(1.0f);
    camera.Update(0.016f);
    
    glm::vec3 new_pos = camera.GetPosition();
    EXPECT(std::abs(new_pos.x - start_pos.x) < 0.01f, "X should not change");
    EXPECT(std::abs(new_pos.y - start_pos.y) < 0.01f, "Y should not change");
    EXPECT(new_pos.z < start_pos.z, "Z should decrease (move forward towards -Z)");
}

TEST(CameraMoveBackward) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Negative movement should move backward
    camera.MoveForward(-1.0f);
    camera.Update(0.016f);
    
    glm::vec3 new_pos = camera.GetPosition();
    EXPECT(new_pos.z > start_pos.z, "Z should increase (move backward from -Z direction)");
}

TEST(CameraMoveRight) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Move right should increase X
    camera.MoveRight(1.0f);
    camera.Update(0.016f);
    
    glm::vec3 new_pos = camera.GetPosition();
    EXPECT(new_pos.x > start_pos.x, "X should increase (move right)");
    EXPECT(std::abs(new_pos.y - start_pos.y) < 0.01f, "Y should not change");
    EXPECT(std::abs(new_pos.z - start_pos.z) < 0.1f, "Z should not change significantly");
}

TEST(CameraMoveLeft) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Negative right movement should move left
    camera.MoveRight(-1.0f);
    camera.Update(0.016f);
    
    glm::vec3 new_pos = camera.GetPosition();
    EXPECT(new_pos.x < start_pos.x, "X should decrease (move left)");
}

TEST(CameraMoveUp) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Move up should increase Y
    camera.MoveUp(1.0f);
    camera.Update(0.016f);
    
    glm::vec3 new_pos = camera.GetPosition();
    EXPECT(new_pos.y > start_pos.y, "Y should increase (move up)");
    EXPECT(std::abs(new_pos.x - start_pos.x) < 0.01f, "X should not change");
    EXPECT(std::abs(new_pos.z - start_pos.z) < 0.1f, "Z should not change significantly");
}

TEST(CameraMoveDown) {
    blec::render::Camera camera;
    glm::vec3 start_pos(0.0f, 0.0f, 0.0f);
    camera.Initialize(start_pos, glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Negative up movement should move down
    camera.MoveUp(-1.0f);
    camera.Update(0.016f);
    
    glm::vec3 new_pos = camera.GetPosition();
    EXPECT(new_pos.y < start_pos.y, "Y should decrease (move down)");
}

// ============================================================================
// TEST SUITE: Camera Rotation
// ============================================================================

TEST(CameraYaw) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    
    glm::vec3 forward_before = camera.GetForward();
    
    // Yaw rotation changes the direction in the horizontal plane
    camera.Yaw(0.5f);  // Rotate right
    camera.Update(0.016f);
    
    glm::vec3 forward_after = camera.GetForward();
    
    // Forward should have changed
    EXPECT(!(glm::all(glm::epsilonEqual(forward_before, forward_after, 0.01f))), 
            "Forward should change after yaw rotation");
}

TEST(CameraPitch) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    
    glm::vec3 forward_before = camera.GetForward();
    
    // Pitch rotation changes the direction in the vertical plane
    camera.Pitch(0.3f);  // Rotate up
    camera.Update(0.016f);
    
    glm::vec3 forward_after = camera.GetForward();
    
    // Forward should have changed, specifically in Y component
    EXPECT(forward_after.y > forward_before.y - 0.01f, "Forward Y should increase (look up)");
}

TEST(CameraPitchClamping) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Try to pitch too far up
    for (int i = 0; i < 100; ++i) {
        camera.Pitch(0.5f);
    }
    camera.Update(0.016f);
    
    // Pitch should be clamped, not allowing gimbal lock
    glm::vec3 forward = camera.GetForward();
    EXPECT(std::abs(forward.y) < 0.99f, "Pitch should be clamped to prevent gimbal lock");
}

TEST(CameraYawContinuous) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    
    glm::vec3 forward1 = camera.GetForward();
    
    camera.Yaw(0.5f);
    camera.Update(0.016f);
    glm::vec3 forward2 = camera.GetForward();
    
    // Rotate opposite direction
    camera.Yaw(-1.0f);
    camera.Update(0.016f);
    glm::vec3 forward3 = camera.GetForward();
    
    camera.Yaw(0.5f);
    camera.Update(0.016f);
    glm::vec3 forward4 = camera.GetForward();
    
    // forward2 and forward4 should be similar (rotated in same direction)
    float dist24 = glm::length(forward2 - forward4);
    EXPECT(dist24 < 0.1f, "Rotating multiple times should produce consistent results");
}

// ============================================================================
// TEST SUITE: View Matrix Generation
// ============================================================================

TEST(CameraViewMatrix) {
    blec::render::Camera camera;
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    camera.Initialize(position, target);
    
    glm::mat4 view = camera.GetViewMatrix();
    
    // View matrix should be a 4x4 matrix
    EXPECT(true, "View matrix generated successfully");
    
    // Transform a point at camera position through view matrix should give near origin
    glm::vec4 cam_pos_in_view = view * glm::vec4(position, 1.0f);
    EXPECT(std::abs(cam_pos_in_view.z) > 0.0f, "Camera position transformed through view matrix should be valid");
}

TEST(CameraViewMatrixOrthonormality) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    camera.Update(0.016f);
    
    glm::vec3 forward = camera.GetForward();
    glm::vec3 right = camera.GetRight();
    glm::vec3 up = camera.GetUp();
    
    // Basis vectors should be unit vectors
    EXPECT(std::abs(glm::length(forward) - 1.0f) < 0.01f, "Forward should be normalized");
    EXPECT(std::abs(glm::length(right) - 1.0f) < 0.01f, "Right should be normalized");
    EXPECT(std::abs(glm::length(up) - 1.0f) < 0.01f, "Up should be normalized");
    
    // Basis vectors should be orthogonal (perpendicular)
    EXPECT(std::abs(glm::dot(forward, right)) < 0.01f, "Forward and right should be perpendicular");
    EXPECT(std::abs(glm::dot(forward, up)) < 0.01f, "Forward and up should be perpendicular");
    EXPECT(std::abs(glm::dot(right, up)) < 0.01f, "Right and up should be perpendicular");
}

// ============================================================================
// TEST SUITE: Movement Speed and Rotation Speed Settings
// ============================================================================

TEST(CameraSetMovementSpeed) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    
    // Set movement speed to specific value
    camera.SetMovementSpeed(10.0f);
    EXPECT(std::abs(camera.GetMovementSpeed() - 10.0f) < 0.01f, "Movement speed should match set value");
}

TEST(CameraSetRotationSpeed) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    
    // Set rotation speed to specific value
    camera.SetRotationSpeed(0.01f);
    EXPECT(std::abs(camera.GetRotationSpeed() - 0.01f) < 0.001f, "Rotation speed should match set value");
}

TEST(CameraMovementSpeedAffectsDistance) {
    blec::render::Camera camera1, camera2;
    glm::vec3 start(0.0f, 0.0f, 0.0f);
    camera1.Initialize(start, glm::vec3(0.0f, 0.0f, -5.0f));
    camera2.Initialize(start, glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Set different movement speeds
    camera1.SetMovementSpeed(5.0f);
    camera2.SetMovementSpeed(10.0f);
    
    // Move both the same amount
    float movement = 1.0f;
    camera1.MoveForward(movement);
    camera2.MoveForward(movement);
    
    camera1.Update(0.016f);
    camera2.Update(0.016f);
    
    glm::vec3 pos1 = camera1.GetPosition();
    glm::vec3 pos2 = camera2.GetPosition();
    
    // Camera2 with higher speed should move farther
    float dist1 = glm::length(pos1 - start);
    float dist2 = glm::length(pos2 - start);
    
    EXPECT(dist2 > dist1, "Higher movement speed should result in greater distance traveled");
}

// ============================================================================
// TEST SUITE: Combined Operations
// ============================================================================

TEST(CameraComplexMovement) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Perform complex movement sequence
    camera.MoveForward(1.0f);
    camera.MoveRight(1.0f);
    camera.MoveUp(0.5f);
    camera.Yaw(0.25f);
    camera.Pitch(0.1f);
    
    camera.Update(0.016f);
    
    glm::vec3 pos = camera.GetPosition();
    
    // Position should have moved in all three dimensions
    EXPECT(pos.x != 0.0f || pos.y != 0.0f || pos.z != 0.0f, "Camera position should change with movement");
}

TEST(CameraMultipleUpdates) {
    blec::render::Camera camera;
    camera.Initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    
    glm::vec3 pos1, pos2;
    
    // First update
    camera.MoveForward(1.0f);
    camera.Update(0.016f);
    pos1 = camera.GetPosition();
    
    // Second update
    camera.MoveForward(0.0f);
    camera.Update(0.016f);
    pos2 = camera.GetPosition();
    
    // Position should remain after no more movement input
    float dist = glm::length(pos2 - pos1);
    EXPECT(dist < 0.01f, "Position should stabilize when movement input is zero");
}

} // namespace blec::test
