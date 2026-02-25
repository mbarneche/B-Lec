/**
 * @file renderer.h
 * @brief Rendering system using OpenGL
 * 
 * Manages all rendering operations, including VAO/VBO management and frame rendering.
 */

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <cstdint>

class Shader;
class World;
class Camera;

/**
 * @class Renderer
 * @brief Handles all rendering operations
 */
class Renderer {
public:
    /**
     * @brief Constructor
     */
    Renderer();

    /**
     * @brief Destructor
     */
    ~Renderer();

    /**
     * @brief Initialize the renderer
     * @param window_width Width of the window
     * @param window_height Height of the window
     * @return true if initialization was successful
     */
    bool Initialize(uint32_t window_width, uint32_t window_height);

    /**
     * @brief Render a frame
     * @param world Pointer to the world to render
     * @param camera Pointer to the camera
     */
    void Render(World* world, Camera* camera);

    /**
     * @brief Clear the screen
     * @param r Red component (0.0-1.0)
     * @param g Green component (0.0-1.0)
     * @param b Blue component (0.0-1.0)
     * @param a Alpha component (0.0-1.0)
     */
    void Clear(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 1.0f);

    /**
     * @brief Render a cube at the specified position
     * @param position Position of the cube in world space
     * @param scale Size of the cube
     * @param color Color of the cube
     */
    void RenderCube(const glm::vec3& position, float scale, const glm::vec3& color);

    /**
     * @brief Set the viewport
     * @param x X position
     * @param y Y position
     * @param width Width in pixels
     * @param height Height in pixels
     */
    void SetViewport(int32_t x, int32_t y, uint32_t width, uint32_t height);

    /**
     * @brief Get the shader program
     * @return Pointer to the default shader
     */
    Shader* GetShader() const { return shader_.get(); }

private:
    /**
     * @brief Initialize the cube mesh
     */
    void InitializeCubeMesh();

    /**
     * @brief Draw currently bound VAO
     * @param index_count Number of indices to draw
     */
    void DrawIndexed(uint32_t index_count);

private:
    std::unique_ptr<Shader> shader_;
    uint32_t cube_vao_;
    uint32_t cube_vbo_;
    uint32_t cube_ebo_;
    uint32_t cube_index_count_;
    uint32_t window_width_;
    uint32_t window_height_;
};
