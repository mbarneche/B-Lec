// render/mesh.h
// 3D mesh management and rendering
// Supports vertex arrays and colored geometry

#ifndef BLEC_RENDER_MESH_H
#define BLEC_RENDER_MESH_H

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace blec {
namespace render {

// Vertex structure for 3D geometry
// Contains position and color information
struct Vertex {
    glm::vec3 position;  // 3D position (X, Y, Z)
    glm::vec3 color;     // RGB color (0-1 range)
    glm::vec3 normal;    // Vertex normal for lighting and culling

    // Default constructor
    Vertex() : position(0), color(1), normal(0, 0, 1) {}

    // Parameterized constructor
    Vertex(const glm::vec3& pos, const glm::vec3& col, const glm::vec3& norm)
        : position(pos), color(col), normal(norm) {}
};

// Mesh represents a collection of vertices and indices
// Describes 3D geometry that can be rendered
class Mesh {
public:
    Mesh();
    ~Mesh();

    // Create a simple colored cube mesh
    // Each face has a different color for visualization
    static Mesh CreateCube();

    // Render the mesh to the screen
    // Requires appropriate projection and view matrices to be set in renderer
    void Render() const;

    // Get number of vertices
    size_t GetVertexCount() const { return vertices_.size(); }

    // Get number of indices (triangles * 3)
    size_t GetIndexCount() const { return indices_.size(); }

    // Enable/disable back-face culling for this mesh
    // When enabled, faces not facing camera won't be rendered
    // Default is disabled for predictable behavior
    void SetBackfaceCulling(bool enabled) { culling_enabled_ = enabled; }

    // Check if back-face culling is enabled
    bool IsBackfaceCullingEnabled() const { return culling_enabled_; }

    // Transform mesh vertices by a transformation matrix
    // Typically used with model matrix for rotation/scale/translation
    void ApplyTransform(const glm::mat4& transform);

private:
    // Vertex data
    std::vector<Vertex> vertices_;

    // Index buffer - tells which vertices make up each triangle
    std::vector<uint32_t> indices_;

    // Whether back-face culling is enabled
    bool culling_enabled_;

    // OpenGL handles (for immediate mode rendering)
    // Will be 0 for immediate mode (glBegin/glEnd)

    // Non-copyable
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Move support for efficient transfer
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    // Render vertices with color and normal information
    void RenderVertices() const;
};

} // namespace render
} // namespace blec

#endif // BLEC_RENDER_MESH_H
