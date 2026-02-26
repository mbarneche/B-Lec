// render/mesh.cpp
// Implementation of 3D mesh creation and rendering

#include "../include/render/mesh.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace blec {
namespace render {

Mesh::Mesh() : culling_enabled_(true) {
}

Mesh::~Mesh() {
    // No OpenGL resources to clean up in immediate mode
}

Mesh Mesh::CreateCube() {
    Mesh cube;

    // Define a unit cube centered at origin (-0.5 to 0.5 on each axis)
    // Each face will have a different color for easy visualization

    const float s = 0.5f;  // Half-size

    // Color for each face (RGB)
    glm::vec3 red(1.0f, 0.0f, 0.0f);      // Front face
    glm::vec3 green(0.0f, 1.0f, 0.0f);    // Back face
    glm::vec3 blue(0.0f, 0.0f, 1.0f);     // Right face
    glm::vec3 yellow(1.0f, 1.0f, 0.0f);   // Left face
    glm::vec3 cyan(0.0f, 1.0f, 1.0f);     // Top face
    glm::vec3 magenta(1.0f, 0.0f, 1.0f);  // Bottom face

    // Front face (Z+)  - RED
    cube.vertices_.push_back(Vertex(glm::vec3(-s, -s,  s), red, glm::vec3(0, 0, 1)));  // 0
    cube.vertices_.push_back(Vertex(glm::vec3( s, -s,  s), red, glm::vec3(0, 0, 1)));  // 1
    cube.vertices_.push_back(Vertex(glm::vec3( s,  s,  s), red, glm::vec3(0, 0, 1)));  // 2
    cube.vertices_.push_back(Vertex(glm::vec3(-s,  s,  s), red, glm::vec3(0, 0, 1)));  // 3

    // Back face (Z-) - GREEN
    cube.vertices_.push_back(Vertex(glm::vec3( s, -s, -s), green, glm::vec3(0, 0, -1)));  // 4
    cube.vertices_.push_back(Vertex(glm::vec3(-s, -s, -s), green, glm::vec3(0, 0, -1)));  // 5
    cube.vertices_.push_back(Vertex(glm::vec3(-s,  s, -s), green, glm::vec3(0, 0, -1)));  // 6
    cube.vertices_.push_back(Vertex(glm::vec3( s,  s, -s), green, glm::vec3(0, 0, -1)));  // 7

    // Right face (X+) - BLUE
    cube.vertices_.push_back(Vertex(glm::vec3( s, -s,  s), blue, glm::vec3(1, 0, 0)));  // 8
    cube.vertices_.push_back(Vertex(glm::vec3( s, -s, -s), blue, glm::vec3(1, 0, 0)));  // 9
    cube.vertices_.push_back(Vertex(glm::vec3( s,  s, -s), blue, glm::vec3(1, 0, 0)));  // 10
    cube.vertices_.push_back(Vertex(glm::vec3( s,  s,  s), blue, glm::vec3(1, 0, 0)));  // 11

    // Left face (X-) - YELLOW
    cube.vertices_.push_back(Vertex(glm::vec3(-s, -s, -s), yellow, glm::vec3(-1, 0, 0)));  // 12
    cube.vertices_.push_back(Vertex(glm::vec3(-s, -s,  s), yellow, glm::vec3(-1, 0, 0)));  // 13
    cube.vertices_.push_back(Vertex(glm::vec3(-s,  s,  s), yellow, glm::vec3(-1, 0, 0)));  // 14
    cube.vertices_.push_back(Vertex(glm::vec3(-s,  s, -s), yellow, glm::vec3(-1, 0, 0)));  // 15

    // Top face (Y+) - CYAN
    cube.vertices_.push_back(Vertex(glm::vec3(-s,  s,  s), cyan, glm::vec3(0, 1, 0)));  // 16
    cube.vertices_.push_back(Vertex(glm::vec3( s,  s,  s), cyan, glm::vec3(0, 1, 0)));  // 17
    cube.vertices_.push_back(Vertex(glm::vec3( s,  s, -s), cyan, glm::vec3(0, 1, 0)));  // 18
    cube.vertices_.push_back(Vertex(glm::vec3(-s,  s, -s), cyan, glm::vec3(0, 1, 0)));  // 19

    // Bottom face (Y-) - MAGENTA
    cube.vertices_.push_back(Vertex(glm::vec3(-s, -s, -s), magenta, glm::vec3(0, -1, 0)));  // 20
    cube.vertices_.push_back(Vertex(glm::vec3( s, -s, -s), magenta, glm::vec3(0, -1, 0)));  // 21
    cube.vertices_.push_back(Vertex(glm::vec3( s, -s,  s), magenta, glm::vec3(0, -1, 0)));  // 22
    cube.vertices_.push_back(Vertex(glm::vec3(-s, -s,  s), magenta, glm::vec3(0, -1, 0)));  // 23

    // Define triangles (2 per face, 12 total)
    // Each face needs proper winding order for back-face culling
    // Counter-clockwise when viewed from outside

    // Front face (0, 1, 2), (0, 2, 3)
    cube.indices_ = {
        0, 1, 2, 0, 2, 3,
        // Back face (4, 5, 6), (4, 6, 7)
        4, 5, 6, 4, 6, 7,
        // Right face (8, 9, 10), (8, 10, 11)
        8, 9, 10, 8, 10, 11,
        // Left face (12, 13, 14), (12, 14, 15)
        12, 13, 14, 12, 14, 15,
        // Top face (16, 17, 18), (16, 18, 19)
        16, 17, 18, 16, 18, 19,
        // Bottom face (20, 21, 22), (20, 22, 23)
        20, 21, 22, 20, 22, 23
    };

    return cube;
}

void Mesh::Render() const {
    // Enable back-face culling if requested
    if (culling_enabled_) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);  // Counter-clockwise is front-facing
    }

    // Use immediate mode for OpenGL 2.1 compatibility
    glBegin(GL_TRIANGLES);

    for (size_t i = 0; i < indices_.size(); ++i) {
        uint32_t idx = indices_[i];
        const Vertex& v = vertices_[idx];

        // Set color for this vertex
        glColor3f(v.color.x, v.color.y, v.color.z);

        // Set normal for lighting (future use)
        glNormal3f(v.normal.x, v.normal.y, v.normal.z);

        // Specify vertex position
        glVertex3f(v.position.x, v.position.y, v.position.z);
    }

    glEnd();

    // Disable culling after rendering
    if (culling_enabled_) {
        glDisable(GL_CULL_FACE);
    }
}

void Mesh::ApplyTransform(const glm::mat4& transform) {
    // Transform all vertices by the matrix
    for (auto& vertex : vertices_) {
        glm::vec4 pos = transform * glm::vec4(vertex.position, 1.0f);
        vertex.position = glm::vec3(pos) / pos.w;

        // For normals, use inverse transpose (3x3)
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));
        vertex.normal = glm::normalize(normalMatrix * vertex.normal);
    }
}

} // namespace render
} // namespace blec
