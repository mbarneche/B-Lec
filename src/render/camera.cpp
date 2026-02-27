// render/camera.cpp
// Implementation of 3D camera with free-flying movement and rotation
#define GLM_ENABLE_EXPERIMENTAL

#include "render/camera.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <cmath>
#include <algorithm>

namespace blec {
namespace render {

Camera::Camera()
    : position_(0.0f, 0.0f, 3.0f)
    , forward_(0.0f, 0.0f, -1.0f)
    , right_(1.0f, 0.0f, 0.0f)
    , up_(0.0f, 1.0f, 0.0f)
    , world_up_(0.0f, 1.0f, 0.0f)
    , yaw_(-glm::half_pi<float>())  // Look down -Z axis
    , pitch_(0.0f)
    , movement_speed_(5.0f)  // 5 units per second
    , rotation_speed_(0.005f)  // radians per input unit (typically pixels)
    , is_moving_(false)
    , movement_input_(0.0f) {
}

void Camera::Initialize(const glm::vec3& position, const glm::vec3& target) {
    position_ = position;
    world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);

    // Calculate forward direction from position to target
    forward_ = glm::normalize(target - position_);

    // Calculate yaw from forward direction
    // Forward = (cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch))
    // We want to find yaw and pitch from forward
    pitch_ = glm::asin(forward_.y);
    yaw_ = glm::atan(forward_.z, forward_.x);

    UpdateVectors();
}

void Camera::MoveForward(float distance) {
    movement_input_.z += distance;
}

void Camera::MoveRight(float distance) {
    movement_input_.x += distance;
}

void Camera::MoveUp(float distance) {
    movement_input_.y += distance;
}

void Camera::Yaw(float input) {
    yaw_ += input * rotation_speed_;
    UpdateVectors();
}

void Camera::Pitch(float input) {
    pitch_ += input * rotation_speed_;

    // Clamp pitch to prevent camera flip (looking too far up/down)
    // Use 89.9 degrees instead of 90 to avoid gimbal lock
    const float pitch_limit = glm::radians(89.9f);
    pitch_ = std::clamp(pitch_, -pitch_limit, pitch_limit);

    UpdateVectors();
}

void Camera::Update(double deltaTime) {
    // Apply queued movement input
    const float dt = static_cast<float>(deltaTime);
    if (movement_input_ != glm::vec3(0.0f)) {
        glm::vec3 movement = (right_ * movement_input_.x) +
                             (world_up_ * movement_input_.y) +
                             (forward_ * movement_input_.z);
        position_ += movement * movement_speed_ * dt;
        is_moving_ = true;
    } else {
        is_moving_ = false;
    }

    // Reset movement input for next frame
    movement_input_ = glm::vec3(0.0f);
}

glm::mat4 Camera::GetViewMatrix() const {
    // View matrix is inverse of camera transform, looking from position towards forward
    return glm::lookAt(position_, position_ + forward_, up_);
}

void Camera::UpdateVectors() {
    // Convert euler angles to forward vector
    // Standard FPS camera math:
    // forward.x = cos(yaw) * cos(pitch)
    // forward.y = sin(pitch)
    // forward.z = sin(yaw) * cos(pitch)

    forward_.x = cos(yaw_) * cos(pitch_);
    forward_.y = sin(pitch_);
    forward_.z = sin(yaw_) * cos(pitch_);

    forward_ = glm::normalize(forward_);

    // Calculate right vector as perpendicular to forward and world up
    right_ = glm::normalize(glm::cross(forward_, world_up_));

    // Calculate actual up vector as perpendicular to forward and right
    up_ = glm::normalize(glm::cross(right_, forward_));
}

} // namespace render
} // namespace blec
