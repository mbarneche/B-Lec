// src/world/block_system.cpp
// Block system implementation with frustum culling

#include "world/block_system.h"
#include <algorithm>
#include <cmath>

namespace blec {
namespace world {

// ============================================================================
// AABB Implementation
// ============================================================================

bool AABB::IntersectsAABB(const AABB& other) const {
    // AABB-AABB intersection test: boxes overlap if they overlap on all axes
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}

// ============================================================================
// ViewFrustum Implementation
// ============================================================================

bool ViewFrustum::IntersectsAABB(const AABB& aabb) const {
    // Frustum-AABB intersection using separating axis theorem
    // Test each frustum plane: if AABB is completely behind any plane, no intersection

    for (int i = 0; i < 6; ++i) {
        const FrustumPlane& plane = planes[i];

        // Get closest point on AABB to plane
        glm::vec3 closest(aabb.min);

        if (plane.normal.x > 0.0f) closest.x = aabb.max.x;
        if (plane.normal.y > 0.0f) closest.y = aabb.max.y;
        if (plane.normal.z > 0.0f) closest.z = aabb.max.z;

        // If closest point is behind plane (negative distance), AABB is fully outside
        float distance = glm::dot(plane.normal, closest) + plane.distance;

        if (distance < 0.0f) {
            return false;  // AABB completely outside this plane
        }
    }

    return true;  // AABB intersects frustum
}

// ============================================================================
// BlockSystem Implementation
// ============================================================================

BlockSystem::BlockSystem()
    : grid_width_(0), grid_height_(0), grid_depth_(0), block_size_(1.0f),
      total_blocks_(0), visible_blocks_(0) {
    // Initialize frustum planes to default values
    for (int i = 0; i < 6; ++i) {
        frustum_.planes[i].normal = glm::vec3(0.0f);
        frustum_.planes[i].distance = 0.0f;
    }
}

bool BlockSystem::Initialize(uint32_t grid_width, uint32_t grid_height, uint32_t grid_depth,
                             float block_size) {
    // Validate parameters
    if (grid_width == 0 || grid_height == 0 || grid_depth == 0 || block_size <= 0.0f) {
        return false;
    }

    grid_width_ = grid_width;
    grid_height_ = grid_height;
    grid_depth_ = grid_depth;
    block_size_ = block_size;

    // Allocate block storage: initialize all blocks as air (type = 0)
    uint32_t total_size = grid_width_ * grid_height_ * grid_depth_;
    blocks_.resize(total_size);
    std::fill(blocks_.begin(), blocks_.end(), Block{0});

    total_blocks_ = 0;
    visible_blocks_ = 0;

    return true;
}

void BlockSystem::CreateTestBlocks() {
    // Create a cube of solid blocks at the center of the grid
    // This provides test geometry for frustum culling

    uint32_t cx = grid_width_ / 2;
    uint32_t cy = grid_height_ / 2;
    uint32_t cz = grid_depth_ / 2;

    // Create 3x3x3 cube of solid blocks centered at grid center
    int offset = 1;
    for (int dx = -offset; dx <= offset; ++dx) {
        for (int dy = -offset; dy <= offset; ++dy) {
            for (int dz = -offset; dz <= offset; ++dz) {
                int32_t x = static_cast<int32_t>(cx) + dx;
                int32_t y = static_cast<int32_t>(cy) + dy;
                int32_t z = static_cast<int32_t>(cz) + dz;

                SetBlock(x, y, z, Block{1});  // Type 1 = solid block
            }
        }
    }
}

int32_t BlockSystem::CoordinatesToIndex(int32_t x, int32_t y, int32_t z) const {
    if (!IsValidCoordinate(x, y, z)) {
        return -1;
    }

    // Row-major indexing: [x + y*width + z*width*height]
    return x + (y * static_cast<int32_t>(grid_width_)) +
           (z * static_cast<int32_t>(grid_width_) * static_cast<int32_t>(grid_height_));
}

bool BlockSystem::IsValidCoordinate(int32_t x, int32_t y, int32_t z) const {
    return x >= 0 && x < static_cast<int32_t>(grid_width_) &&
           y >= 0 && y < static_cast<int32_t>(grid_height_) &&
           z >= 0 && z < static_cast<int32_t>(grid_depth_);
}

Block BlockSystem::GetBlock(int32_t x, int32_t y, int32_t z) const {
    int32_t idx = CoordinatesToIndex(x, y, z);
    if (idx < 0) {
        return Block{0};  // Out of bounds = air
    }

    return blocks_[idx];
}

bool BlockSystem::SetBlock(int32_t x, int32_t y, int32_t z, Block block) {
    int32_t idx = CoordinatesToIndex(x, y, z);
    if (idx < 0) {
        return false;  // Out of bounds
    }

    const uint8_t previous_type = blocks_[idx].type;
    blocks_[idx] = block;

    if (previous_type == 0 && block.type != 0) {
        total_blocks_ += 1;
    } else if (previous_type != 0 && block.type == 0) {
        total_blocks_ -= 1;
    }

    return true;
}

glm::vec3 BlockSystem::GetBlockWorldPosition(int32_t grid_x, int32_t grid_y,
                                             int32_t grid_z) const {
    // Convert grid coordinates to world position
    // Grid origin at (0, 0, 0) in world space
    return glm::vec3(grid_x * block_size_, grid_y * block_size_, grid_z * block_size_);
}

AABB BlockSystem::GetBlockAABB(int32_t grid_x, int32_t grid_y, int32_t grid_z) const {
    glm::vec3 world_pos = GetBlockWorldPosition(grid_x, grid_y, grid_z);
    return AABB{
        world_pos,                                              // min
        world_pos + glm::vec3(block_size_, block_size_, block_size_)  // max
    };
}

void BlockSystem::RecalculateTotalBlockCount() {
    // Count all non-air blocks
    uint32_t count = 0;
    for (const auto& block : blocks_) {
        if (block.type != 0) {
            count++;
        }
    }
    total_blocks_ = count;
}

void BlockSystem::ExtractFrustum(const glm::mat4& view_matrix,
                                  const glm::mat4& projection_matrix) {
    // Combine projection and view matrices
    glm::mat4 clip_matrix = projection_matrix * view_matrix;

    // Extract frustum planes from clip matrix
    // Each row of clip matrix contains plane equations after normalization

    // Left plane: clip_matrix[3] + clip_matrix[0]
    {
        glm::vec4 plane_eq = clip_matrix[3] + clip_matrix[0];
        glm::vec3 normal(plane_eq.x, plane_eq.y, plane_eq.z);
        float length = glm::length(normal);
        if (length > 0.0001f) {
            normal /= length;
            frustum_.planes[2].normal = normal;  // Left plane
            frustum_.planes[2].distance = plane_eq.w / length;
        }
    }

    // Right plane: clip_matrix[3] - clip_matrix[0]
    {
        glm::vec4 plane_eq = clip_matrix[3] - clip_matrix[0];
        glm::vec3 normal(plane_eq.x, plane_eq.y, plane_eq.z);
        float length = glm::length(normal);
        if (length > 0.0001f) {
            normal /= length;
            frustum_.planes[3].normal = normal;  // Right plane
            frustum_.planes[3].distance = plane_eq.w / length;
        }
    }

    // Top plane: clip_matrix[3] - clip_matrix[1]
    {
        glm::vec4 plane_eq = clip_matrix[3] - clip_matrix[1];
        glm::vec3 normal(plane_eq.x, plane_eq.y, plane_eq.z);
        float length = glm::length(normal);
        if (length > 0.0001f) {
            normal /= length;
            frustum_.planes[4].normal = normal;  // Top plane
            frustum_.planes[4].distance = plane_eq.w / length;
        }
    }

    // Bottom plane: clip_matrix[3] + clip_matrix[1]
    {
        glm::vec4 plane_eq = clip_matrix[3] + clip_matrix[1];
        glm::vec3 normal(plane_eq.x, plane_eq.y, plane_eq.z);
        float length = glm::length(normal);
        if (length > 0.0001f) {
            normal /= length;
            frustum_.planes[5].normal = normal;  // Bottom plane
            frustum_.planes[5].distance = plane_eq.w / length;
        }
    }

    // Near plane: clip_matrix[3] + clip_matrix[2]
    {
        glm::vec4 plane_eq = clip_matrix[3] + clip_matrix[2];
        glm::vec3 normal(plane_eq.x, plane_eq.y, plane_eq.z);
        float length = glm::length(normal);
        if (length > 0.0001f) {
            normal /= length;
            frustum_.planes[0].normal = normal;  // Near plane
            frustum_.planes[0].distance = plane_eq.w / length;
        }
    }

    // Far plane: clip_matrix[3] - clip_matrix[2]
    {
        glm::vec4 plane_eq = clip_matrix[3] - clip_matrix[2];
        glm::vec3 normal(plane_eq.x, plane_eq.y, plane_eq.z);
        float length = glm::length(normal);
        if (length > 0.0001f) {
            normal /= length;
            frustum_.planes[1].normal = normal;  // Far plane
            frustum_.planes[1].distance = plane_eq.w / length;
        }
    }
}

void BlockSystem::UpdateVisibility() {
    // Count non-air blocks visible in frustum
    uint32_t visible_count = 0;

    for (uint32_t z = 0; z < grid_depth_; ++z) {
        for (uint32_t y = 0; y < grid_height_; ++y) {
            for (uint32_t x = 0; x < grid_width_; ++x) {
                const Block& block = GetBlock(x, y, z);

                // Only test non-air blocks
                if (block.type != 0) {
                    AABB block_aabb = GetBlockAABB(x, y, z);

                    // Test against frustum
                    if (frustum_.IntersectsAABB(block_aabb)) {
                        visible_count++;
                    }
                }
            }
        }
    }

    visible_blocks_ = visible_count;
}

} // namespace world
} // namespace blec
