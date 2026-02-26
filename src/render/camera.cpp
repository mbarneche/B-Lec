// render/camera.cpp
// Implementation of 3D camera with free-flying movement and rotation
#define GLM_ENABLE_EXPERIMENTAL

#include "render/camera.h"
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
    , yaw_(-90.0f)  // Look down -Z axis
    , pitch_(0.0f)
    , movement_speed_(5.0f)  // 5 units per second
    , rotation_speed_(0.005f)  // radians per pixel
    , is_moving_(false) {
}

void Camera::Initialize(const glm::vec3& position, const glm::vec3& target) {
    position_ = position;
    world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);

    // Calculate forward direction from position to target
    forward_ = glm::normalize(target - position_);

    // Calculate yaw from forward direction
    // Forward = (cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch))
    // We want to find yaw and pitch from forward
    float pitch = glm::asin(forward_.y);
    float yaw = glm::atan(forward_.z, forward_.x);

    pitch_ = glm::degrees(pitch);
    yaw_ = glm::degrees(yaw);

    UpdateVectors();
}

void Camera::MoveForward(float distance) {
    position_ += forward_ * distance;
    is_moving_ = true;
}

void Camera::MoveRight(float distance) {
    position_ += right_ * distance;
    is_moving_ = true;
}

void Camera::MoveUp(float distance) {
    position_ += world_up_ * distance;
    is_moving_ = true;
}

void Camera::Yaw(float angleRadians) {
    yaw_ += glm::degrees(angleRadians);
    UpdateVectors();
}

void Camera::Pitch(float angleRadians) {
    pitch_ += glm::degrees(angleRadians);

    // Clamp pitch to prevent camera flip (looking too far up/down)
    // Use 89.9 degrees instead of 90 to avoid gimbal lock
    pitch_ = std::clamp(pitch_, -89.9f, 89.9f);

    UpdateVectors();
}

void Camera::Update(double deltaTime) {
    // Reset movement flag for this frame
    is_moving_ = false;
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

    float yawRad = glm::radians(yaw_);
    float pitchRad = glm::radians(pitch_);

    forward_.x = cos(yawRad) * cos(pitchRad);
    forward_.y = sin(pitchRad);
    forward_.z = sin(yawRad) * cos(pitchRad);

    forward_ = glm::normalize(forward_);

    // Calculate right vector as perpendicular to forward and world up
    right_ = glm::normalize(glm::cross(forward_, world_up_));

    // Calculate actual up vector as perpendicular to forward and right
    up_ = glm::normalize(glm::cross(right_, forward_));
}

} // namespace render
} // namespace blec
