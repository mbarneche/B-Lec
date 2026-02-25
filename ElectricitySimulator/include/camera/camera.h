/**
 * @file camera.h
 * @brief Camera system for 3D visualization
 * 
 * Manages the player's view into the 3D world.
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class Camera
 * @brief Represents a camera in 3D space
 */
class Camera {
public:
    /**
     * @brief Constructor
     * @param position Initial position
     * @param target Initial target/look-at point
     * @param up Up vector
     */
    Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

    /**
     * @brief Update camera from input
     * @param delta_time Time since last frame
     */
    void Update(float delta_time);

    /**
     * @brief Get the view matrix
     * @return The view matrix
     */
    glm::mat4 GetViewMatrix() const;

    /**
     * @brief Get the projection matrix
     * @return The projection matrix
     */
    glm::mat4 GetProjectionMatrix() const;

    /**
     * @brief Set the position of the camera
     * @param position The new position
     */
    void SetPosition(const glm::vec3& position) { position_ = position; }

    /**
     * @brief Get the position of the camera
     * @return The camera position
     */
    glm::vec3 GetPosition() const { return position_; }

    /**
     * @brief Get the forward direction of the camera
     * @return The forward vector
     */
    glm::vec3 GetForward() const { return glm::normalize(target_ - position_); }

    /**
     * @brief Get the right direction of the camera
     * @return The right vector
     */
    glm::vec3 GetRight() const { return glm::normalize(glm::cross(GetForward(), up_)); }

    /**
     * @brief Move the camera forward/backward
     * @param distance Distance to move
     */
    void MoveForward(float distance) {
        position_ += GetForward() * distance;
        target_ += GetForward() * distance;
    }

    /**
     * @brief Move the camera left/right
     * @param distance Distance to move
     */
    void MoveRight(float distance) {
        position_ += GetRight() * distance;
        target_ += GetRight() * distance;
    }

    /**
     * @brief Move the camera up/down
     * @param distance Distance to move
     */
    void MoveUp(float distance) {
        position_ += up_ * distance;
        target_ += up_ * distance;
    }

    /**
     * @brief Rotate the camera around the target point
     * @param yaw Rotation around Y axis in degrees
     * @param pitch Rotation around X axis in degrees
     */
    void Rotate(float yaw, float pitch);

    /**
     * @brief Get the window aspect ratio
     * @return Aspect ratio
     */
    float GetAspectRatio() const { return aspect_ratio_; }

    /**
     * @brief Set the window aspect ratio
     * @param ratio The new aspect ratio
     */
    void SetAspectRatio(float ratio) { aspect_ratio_ = ratio; }

private:
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 up_;
    float fov_;
    float near_plane_;
    float far_plane_;
    float aspect_ratio_;
    float yaw_;
    float pitch_;
};
