/**
 * @file camera.cpp
 * @brief Camera implementation
 */

#include "camera/camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
    : position_(position), target_(target), up_(glm::normalize(up)),
      fov_(45.0f), near_plane_(0.1f), far_plane_(500.0f),
      aspect_ratio_(16.0f / 9.0f), yaw_(0.0f), pitch_(0.0f) {
}

void Camera::Update(float delta_time) {
    // Camera update logic can be added here if needed
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position_, target_, up_);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(fov_), aspect_ratio_, near_plane_, far_plane_);
}

void Camera::Rotate(float yaw, float pitch) {
    yaw_ += yaw;
    pitch_ += pitch;

    // Clamp pitch to prevent flipping
    if (pitch_ > 89.0f) {
        pitch_ = 89.0f;
    }
    if (pitch_ < -89.0f) {
        pitch_ = -89.0f;
    }

    // Calculate new target based on spherical coordinates
    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
    direction.y = glm::sin(glm::radians(pitch_));
    direction.z = glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));

    target_ = position_ + glm::normalize(direction);
}
