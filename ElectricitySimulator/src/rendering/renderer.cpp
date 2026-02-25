/**
 * @file renderer.cpp
 * @brief Renderer implementation
 */

#include "rendering/renderer.h"
#include "rendering/shader.h"
#include "world/world.h"
#include "world/chunk.h"
#include "camera/camera.h"
#include <glad/glad.h>
#include <iostream>

// Simple vertex shader
const char* VERTEX_SHADER = R"(
#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vertexColor;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertexColor = color;
}
)";

// Simple fragment shader
const char* FRAGMENT_SHADER = R"(
#version 460 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

Renderer::Renderer()
    : cube_vao_(0), cube_vbo_(0), cube_ebo_(0), cube_index_count_(0),
      window_width_(1280), window_height_(720) {
}

Renderer::~Renderer() {
    if (cube_vao_ != 0) {
        glDeleteVertexArrays(1, &cube_vao_);
    }
    if (cube_vbo_ != 0) {
        glDeleteBuffers(1, &cube_vbo_);
    }
    if (cube_ebo_ != 0) {
        glDeleteBuffers(1, &cube_ebo_);
    }
}

bool Renderer::Initialize(uint32_t window_width, uint32_t window_height) {
    window_width_ = window_width;
    window_height_ = window_height;

    // Initialize GLAD
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Create shader program
    shader_ = std::make_unique<Shader>(VERTEX_SHADER, FRAGMENT_SHADER);

    // Initialize cube mesh
    InitializeCubeMesh();

    return true;
}

void Renderer::InitializeCubeMesh() {
    // Cube vertices
    float vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // Back face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    uint32_t indices[] = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Back
        5, 4, 7, 7, 6, 5,
        // Left
        4, 0, 3, 3, 7, 4,
        // Right
        1, 5, 6, 6, 2, 1,
        // Top
        3, 2, 6, 6, 7, 3,
        // Bottom
        4, 5, 1, 1, 0, 4,
    };

    cube_index_count_ = 36;

    glGenVertexArrays(1, &cube_vao_);
    glGenBuffers(1, &cube_vbo_);
    glGenBuffers(1, &cube_ebo_);

    glBindVertexArray(cube_vao_);

    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::Render(World* world, Camera* camera) {
    shader_->Use();

    glm::mat4 projection = camera->GetProjectionMatrix();
    glm::mat4 view = camera->GetViewMatrix();

    shader_->SetMatrix4("projection", projection);
    shader_->SetMatrix4("view", view);

    // Render chunks
    if (world) {
        auto chunks = world->GetAllChunks();
        for (Chunk* chunk : chunks) {
            glm::ivec3 chunk_pos = chunk->GetPosition();
            glm::vec3 world_pos = glm::vec3(chunk_pos) * static_cast<float>(CHUNK_SIZE);

            // Render blocks in this chunk
            for (uint32_t x = 0; x < CHUNK_SIZE; ++x) {
                for (uint32_t y = 0; y < CHUNK_SIZE; ++y) {
                    for (uint32_t z = 0; z < CHUNK_SIZE; ++z) {
                        Block* block = chunk->GetBlock(x, y, z);
                        if (block && block->IsSolid()) {
                            glm::vec3 block_pos = world_pos + glm::vec3(x, y, z);
                            glm::vec3 color = BlockRegistry::GetBlockColor(block->type, block->is_powered);
                            RenderCube(block_pos, 1.0f, color);
                        }
                    }
                }
            }
        }
    }
}

void Renderer::RenderCube(const glm::vec3& position, float scale, const glm::vec3& color) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, glm::vec3(scale));

    shader_->SetMatrix4("model", model);

    glBindVertexArray(cube_vao_);
    glDrawElements(GL_TRIANGLES, cube_index_count_, GL_UNSIGNED_INT, 0);
}

void Renderer::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(int32_t x, int32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height);
}

void Renderer::DrawIndexed(uint32_t index_count) {
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}
