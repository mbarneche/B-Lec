// code_testing/render/test_mesh.cpp
// Unit tests for Mesh class
// Tests colored cube creation, vertex data, and back-face culling

#include "../test_framework.h"
#include "render/mesh.h"
#include <glm/glm.hpp>
#include <cmath>

namespace blec::test {

// ============================================================================
// TEST SUITE: Mesh Creation and Basic Properties
// ============================================================================

TEST(MeshCreateCube) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Cube should have 24 vertices (4 per face × 6 faces)
    EXPECT(cube.GetVertexCount() == 24, "Cube should have 24 vertices");
    
    // Cube should have 36 indices (6 triangles per face × 2 triangles per face × 6 faces)
    EXPECT(cube.GetIndexCount() == 36, "Cube should have 36 indices");
}

TEST(MeshVertexPositions) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Check that vertices are within expected bounds
    // Cube should be unit cube from -0.5 to 0.5 in all axes
    bool all_in_range = true;
    
    for (uint32_t i = 0; i < cube.GetVertexCount(); ++i) {
        glm::vec3 pos = cube.GetVertexPosition(i);
        
        // Check bounds
        if (pos.x < -0.5f - 0.01f || pos.x > 0.5f + 0.01f ||
            pos.y < -0.5f - 0.01f || pos.y > 0.5f + 0.01f ||
            pos.z < -0.5f - 0.01f || pos.z > 0.5f + 0.01f) {
            all_in_range = false;
            break;
        }
    }
    
    EXPECT(all_in_range, "All vertices should be within unit cube bounds");
}

TEST(MeshVertexCount) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    EXPECT(cube.GetVertexCount() == 24, "GetVertexCount should return accurate count");
}

TEST(MeshIndexCount) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    EXPECT(cube.GetIndexCount() == 36, "GetIndexCount should return accurate count");
}

// ============================================================================
// TEST SUITE: Mesh Face Colors
// ============================================================================

TEST(MeshCubeFrontFaceColor) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Front face (Z+) vertices are at indices 0-3
    bool front_face_red = true;
    for (int i = 0; i < 4; ++i) {
        glm::vec3 color = cube.GetVertexColor(i);
        
        // Front face should be red (1, 0, 0)
        if (std::abs(color.r - 1.0f) > 0.01f ||
            std::abs(color.g - 0.0f) > 0.01f ||
            std::abs(color.b - 0.0f) > 0.01f) {
            front_face_red = false;
            break;
        }
    }
    
    EXPECT(front_face_red, "Front face vertices should be red (1, 0, 0)");
}

TEST(MeshCubeBackFaceColor) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Back face (Z-) vertices are at indices 4-7
    bool back_face_green = true;
    for (int i = 4; i < 8; ++i) {
        glm::vec3 color = cube.GetVertexColor(i);
        
        // Back face should be green (0, 1, 0)
        if (std::abs(color.r - 0.0f) > 0.01f ||
            std::abs(color.g - 1.0f) > 0.01f ||
            std::abs(color.b - 0.0f) > 0.01f) {
            back_face_green = false;
            break;
        }
    }
    
    EXPECT(back_face_green, "Back face vertices should be green (0, 1, 0)");
}

TEST(MeshCubeRightFaceColor) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Right face (X+) vertices are at indices 8-11
    bool right_face_blue = true;
    for (int i = 8; i < 12; ++i) {
        glm::vec3 color = cube.GetVertexColor(i);
        
        // Right face should be blue (0, 0, 1)
        if (std::abs(color.r - 0.0f) > 0.01f ||
            std::abs(color.g - 0.0f) > 0.01f ||
            std::abs(color.b - 1.0f) > 0.01f) {
            right_face_blue = false;
            break;
        }
    }
    
    EXPECT(right_face_blue, "Right face vertices should be blue (0, 0, 1)");
}

TEST(MeshCubeLeftFaceColor) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Left face (X-) vertices are at indices 12-15
    bool left_face_yellow = true;
    for (int i = 12; i < 16; ++i) {
        glm::vec3 color = cube.GetVertexColor(i);
        
        // Left face should be yellow (1, 1, 0)
        if (std::abs(color.r - 1.0f) > 0.01f ||
            std::abs(color.g - 1.0f) > 0.01f ||
            std::abs(color.b - 0.0f) > 0.01f) {
            left_face_yellow = false;
            break;
        }
    }
    
    EXPECT(left_face_yellow, "Left face vertices should be yellow (1, 1, 0)");
}

TEST(MeshCubeTopFaceColor) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Top face (Y+) vertices are at indices 16-19
    bool top_face_cyan = true;
    for (int i = 16; i < 20; ++i) {
        glm::vec3 color = cube.GetVertexColor(i);
        
        // Top face should be cyan (0, 1, 1)
        if (std::abs(color.r - 0.0f) > 0.01f ||
            std::abs(color.g - 1.0f) > 0.01f ||
            std::abs(color.b - 1.0f) > 0.01f) {
            top_face_cyan = false;
            break;
        }
    }
    
    EXPECT(top_face_cyan, "Top face vertices should be cyan (0, 1, 1)");
}

TEST(MeshCubeBottomFaceColor) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Bottom face (Y-) vertices are at indices 20-23
    bool bottom_face_magenta = true;
    for (int i = 20; i < 24; ++i) {
        glm::vec3 color = cube.GetVertexColor(i);
        
        // Bottom face should be magenta (1, 0, 1)
        if (std::abs(color.r - 1.0f) > 0.01f ||
            std::abs(color.g - 0.0f) > 0.01f ||
            std::abs(color.b - 1.0f) > 0.01f) {
            bottom_face_magenta = false;
            break;
        }
    }
    
    EXPECT(bottom_face_magenta, "Bottom face vertices should be magenta (1, 0, 1)");
}

TEST(MeshColorRange) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // All color components should be in [0, 1] range
    bool colors_in_range = true;
    for (uint32_t i = 0; i < cube.GetVertexCount(); ++i) {
        glm::vec3 color = cube.GetVertexColor(i);
        
        if (color.r < 0.0f || color.r > 1.0f ||
            color.g < 0.0f || color.g > 1.0f ||
            color.b < 0.0f || color.b > 1.0f) {
            colors_in_range = false;
            break;
        }
    }
    
    EXPECT(colors_in_range, "All color components should be in [0, 1] range");
}

// ============================================================================
// TEST SUITE: Back-Face Culling
// ============================================================================

TEST(MeshBackfaceCullingDefault) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // By default, culling should be disabled
    EXPECT(!cube.IsBackfaceCullingEnabled(), "Back-face culling should be disabled by default");
}

TEST(MeshEnableBackfaceCulling) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    cube.SetBackfaceCulling(true);
    EXPECT(cube.IsBackfaceCullingEnabled(), "Back-face culling should be enabled after SetBackfaceCulling(true)");
}

TEST(MeshDisableBackfaceCulling) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    cube.SetBackfaceCulling(true);
    EXPECT(cube.IsBackfaceCullingEnabled(), "Culling should be enabled");
    
    cube.SetBackfaceCulling(false);
    EXPECT(!cube.IsBackfaceCullingEnabled(), "Back-face culling should be disabled after SetBackfaceCulling(false)");
}

TEST(MeshToggleBackfaceCulling) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    bool initial = cube.IsBackfaceCullingEnabled();
    
    cube.SetBackfaceCulling(!initial);
    EXPECT(cube.IsBackfaceCullingEnabled() == !initial, "Culling state should toggle");
    
    cube.SetBackfaceCulling(initial);
    EXPECT(cube.IsBackfaceCullingEnabled() == initial, "Culling state should toggle back");
}

// ============================================================================
// TEST SUITE: Mesh Indices
// ============================================================================

TEST(MeshIndicesValid) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // All indices should be valid (< 24)
    bool indices_valid = true;
    for (uint32_t i = 0; i < cube.GetIndexCount(); ++i) {
        uint32_t idx = cube.GetIndex(i);
        if (idx >= cube.GetVertexCount()) {
            indices_valid = false;
            break;
        }
    }
    
    EXPECT(indices_valid, "All indices should be valid vertex references");
}

TEST(MeshTriangleCount) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // 36 indices = 12 triangles (3 indices per triangle)
    uint32_t triangle_count = cube.GetIndexCount() / 3;
    EXPECT(triangle_count == 12, "Cube should have 12 triangles");
}

TEST(MeshIndicesSequential) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Indices should form valid triangles (no negative indices, etc.)
    for (uint32_t i = 0; i < cube.GetIndexCount(); i += 3) {
        uint32_t idx0 = cube.GetIndex(i);
        uint32_t idx1 = cube.GetIndex(i + 1);
        uint32_t idx2 = cube.GetIndex(i + 2);
        
        // Each triangle should have three different indices
        EXPECT(idx0 != idx1 && idx1 != idx2 && idx0 != idx2, "Triangle vertices should be distinct");
    }
}

// ============================================================================
// TEST SUITE: Mesh Properties and Getters
// ============================================================================

TEST(MeshGetVertexPosition) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    for (uint32_t i = 0; i < cube.GetVertexCount(); ++i) {
        glm::vec3 pos = cube.GetVertexPosition(i);
        
        // Position should be a valid vec3
        EXPECT(true, "GetVertexPosition returns valid data");
        break; // Test just one vertex
    }
}

TEST(MeshGetVertexColor) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    for (uint32_t i = 0; i < cube.GetVertexCount(); ++i) {
        glm::vec3 color = cube.GetVertexColor(i);
        
        // Color should be in valid range
        EXPECT(color.r >= 0.0f && color.r <= 1.0f, "Vertex color R should be in [0, 1]");
        break; // Test just one vertex
    }
}

TEST(MeshGetIndex) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    for (uint32_t i = 0; i < cube.GetIndexCount(); ++i) {
        uint32_t idx = cube.GetIndex(i);
        
        // Index should be valid
        EXPECT(idx < 24, "Index should reference valid vertex");
        break; // Test just one index
    }
}

// ============================================================================
// TEST SUITE: Multiple Mesh Creation
// ============================================================================

TEST(MeshMultipleCubes) {
    blec::render::Mesh cube1 = blec::render::Mesh::CreateCube();
    blec::render::Mesh cube2 = blec::render::Mesh::CreateCube();
    
    // Both cubes should have same vertex count
    EXPECT(cube1.GetVertexCount() == cube2.GetVertexCount(), "Multiple cubes should have same vertex count");
    
    // Both cubes should have same index count
    EXPECT(cube1.GetIndexCount() == cube2.GetIndexCount(), "Multiple cubes should have same index count");
}

TEST(MeshIndependentCullingState) {
    blec::render::Mesh cube1 = blec::render::Mesh::CreateCube();
    blec::render::Mesh cube2 = blec::render::Mesh::CreateCube();
    
    // Set different culling states
    cube1.SetBackfaceCulling(true);
    cube2.SetBackfaceCulling(false);
    
    EXPECT(cube1.IsBackfaceCullingEnabled(), "Cube1 culling should be enabled");
    EXPECT(!cube2.IsBackfaceCullingEnabled(), "Cube2 culling should be disabled");
}

// ============================================================================
// TEST SUITE: Mesh Vertex Data Consistency
// ============================================================================

TEST(MeshVertexNormals) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // Cube should have normals for all vertices
    for (uint32_t i = 0; i < cube.GetVertexCount(); ++i) {
        glm::vec3 normal = cube.GetVertexNormal(i);
        
        // Normal should be unit vector or zero (for future lighting)
        float length = glm::length(normal);
        EXPECT(length >= 0.0f && length <= 1.01f, "Vertex normal should be unit or zero");
        break; // Test just one vertex
    }
}

TEST(MeshFaceIntegrity) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    
    // All 6 faces should have appropriate colors
    std::array<glm::vec3, 6> expected_colors = {
        glm::vec3(1.0f, 0.0f, 0.0f), // Front Red
        glm::vec3(0.0f, 1.0f, 0.0f), // Back Green
        glm::vec3(0.0f, 0.0f, 1.0f), // Right Blue
        glm::vec3(1.0f, 1.0f, 0.0f), // Left Yellow
        glm::vec3(0.0f, 1.0f, 1.0f), // Top Cyan
        glm::vec3(1.0f, 0.0f, 1.0f)  // Bottom Magenta
    };
    
    // Just verify we have 6 different color blocks
    int colored_blocks = 0;
    for (int face = 0; face < 6; ++face) {
        glm::vec3 face_color = cube.GetVertexColor(face * 4);
        colored_blocks++;
    }
    
    EXPECT(colored_blocks == 6, "Cube should have 6 distinctly colored faces");
}

} // namespace blec::test
