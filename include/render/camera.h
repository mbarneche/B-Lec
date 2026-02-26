// render/camera.h
// Free-flying 3D camera with keyboard and mouse control
// Supports WASD movement and mouse-look rotation

#ifndef BLEC_RENDER_CAMERA_H
#define BLEC_RENDER_CAMERA_H

#include <glm/glm.hpp>

namespace blec {
namespace render {

// Camera represents a 3D view into the scene
// Supports free-flying movement and rotation (first-person style)
class Camera {
public:
    // Default constructor - camera starts at origin looking down -Z
    Camera();

    // Initialize camera with position and look-at target
    void Initialize(const glm::vec3& position, const glm::vec3& target);

    // Move camera forward/backward along its forward vector
    // positive distance = forward, negative = backward
    void MoveForward(float distance);

    // Move camera left/right along its right vector
    // positive distance = right, negative = left (strafe)
    void MoveRight(float distance);

    // Move camera up/down along world up vector
    // positive distance = up, negative = down
    void MoveUp(float distance);

    // Rotate camera around Y axis (horizontal rotation)
    // positive angle = counterclockwise (left), negative = clockwise (right)
    void Yaw(float angleRadians);

    // Rotate camera around its right vector (vertical rotation)
    // positive angle = look up, negative = look down
    // Clamped to prevent flipping (-89.9 to 89.9 degrees)
    void Pitch(float angleRadians);

    // Update camera based on delta time
    // Should be called once per frame
    void Update(double deltaTime);

    // Get the view matrix for rendering
    glm::mat4 GetViewMatrix() const;

    // Get camera position
    glm::vec3 GetPosition() const { return position_; }

    // Get forward direction
    glm::vec3 GetForward() const { return forward_; }

    // Get right direction
    glm::vec3 GetRight() const { return right_; }

    // Get up direction
    glm::vec3 GetUp() const { return up_; }

    // Get yaw rotation in radians (horizontal rotation around world up axis)
    float GetYaw() const { return yaw_; }

    // Get pitch rotation in radians (vertical rotation around camera right axis)
    float GetPitch() const { return pitch_; }

    // Set camera movement speed (units per second)
    void SetMovementSpeed(float speed) { movement_speed_ = speed; }

    // Set camera rotation speed (radians per pixel of mouse movement)
    void SetRotationSpeed(float speed) { rotation_speed_ = speed; }

    // Check if movement keys are still being processed
    // Returns true if camera made any movement this frame
    bool IsMoving() const { return is_moving_; }

private:
    // Camera position in world space
    glm::vec3 position_;

    // Camera forward direction (where camera is looking)
    glm::vec3 forward_;

    // Camera right direction (perpendicular to forward and up)
    glm::vec3 right_;

    // Camera up direction (perpendicular to forward and right)
    glm::vec3 up_;

    // World up vector (always points up, used as reference)
    glm::vec3 world_up_;

    // Euler angles for rotation state
    float yaw_;      // Rotation around Y axis
    float pitch_;    // Rotation around right axis

    // Movement parameters
    float movement_speed_;  // Units per second
    float rotation_speed_;  // Radians per pixel

    // State tracking
    bool is_moving_;  // Whether camera moved this frame

    // Recalculate forward, right, up vectors from euler angles
    void UpdateVectors();

    // Non-copyable
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
};

} // namespace render
} // namespace blec

#endif // BLEC_RENDER_CAMERA_H
