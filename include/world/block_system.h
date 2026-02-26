// include/world/block_system.h
// Block system for managing voxel grid and frustum culling
// Tracks blocks in the world and tests visibility against camera frustum

#ifndef BLEC_WORLD_BLOCK_SYSTEM_H
#define BLEC_WORLD_BLOCK_SYSTEM_H

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace blec {
namespace world {

/// Represents a single block in the voxel grid
/// Air blocks (value 0) are invisible, non-air blocks (value > 0) are visible
struct Block {
    uint8_t type;  // 0 = air, 1+ = solid block type
};

/// Frustum plane equation: ax + by + cz + d = 0
struct FrustumPlane {
    glm::vec3 normal;  // Normal vector (a, b, c)
    float distance;    // Distance constant (d)
};

/// Axis-aligned bounding box for intersection testing
struct AABB {
    glm::vec3 min;  // Minimum coordinate
    glm::vec3 max;  // Maximum coordinate

    /// Check if this AABB intersects another AABB
    bool IntersectsAABB(const AABB& other) const;
};

/// View frustum with 6 planes (near, far, left, right, top, bottom)
struct ViewFrustum {
    FrustumPlane planes[6];  // 0:near, 1:far, 2:left, 3:right, 4:top, 5:bottom

    /// Check if AABB intersects this frustum
    /// Efficiently tests AABB against frustum planes
    bool IntersectsAABB(const AABB& aabb) const;
};

/// Block system managing voxel grid and visibility queries
/// Provides frustum culling for efficient block rendering
class BlockSystem {
public:
    /// Default constructor - creates empty block system
    BlockSystem();

    /// Destructor
    ~BlockSystem() = default;

    /// Initialize block system with grid dimensions
    /// @param grid_width: Width of grid in blocks (X axis)
    /// @param grid_height: Height of grid in blocks (Y axis)
    /// @param grid_depth: Depth of grid in blocks (Z axis)
    /// @param block_size: Physical size of each block in world units (default 1.0)
    /// @return true if initialization successful, false if dimensions invalid
    bool Initialize(uint32_t grid_width, uint32_t grid_height, uint32_t grid_depth,
                    float block_size = 1.0f);

    /// Create initial test blocks (cube at origin)
    /// Used for testing and demonstration
    void CreateTestBlocks();

    /// Extract view frustum from view-projection matrix
    /// Should be called once per frame with current camera matrices
    /// @param view_matrix: Camera view matrix (from camera)
    /// @param projection_matrix: Perspective projection matrix
    void ExtractFrustum(const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

    /// Update visibility counts based on extracted frustum
    /// Counts how many non-air blocks are visible in camera view
    /// Should be called each frame after ExtractFrustum
    void UpdateVisibility();

    /// Get current view frustum (for testing)
    const ViewFrustum& GetFrustum() const { return frustum_; }

    /// Get number of total non-air blocks in world
    uint32_t GetTotalBlockCount() const { return total_blocks_; }

    /// Get number of non-air blocks visible in frustum
    uint32_t GetVisibleBlockCount() const { return visible_blocks_; }

    /// Get block at grid position
    /// @param x, y, z: Grid coordinates
    /// @return Block at position, or Block{0} (air) if out of bounds
    Block GetBlock(int32_t x, int32_t y, int32_t z) const;

    /// Set block at grid position
    /// @param x, y, z: Grid coordinates
    /// @param block: Block to set
    /// @return true if successfully set, false if out of bounds
    bool SetBlock(int32_t x, int32_t y, int32_t z, Block block);

    /// Get grid dimensions
    uint32_t GetGridWidth() const { return grid_width_; }
    uint32_t GetGridHeight() const { return grid_height_; }
    uint32_t GetGridDepth() const { return grid_depth_; }

    /// Get block size in world units
    float GetBlockSize() const { return block_size_; }

    /// Get world position of block corner
    /// @param grid_x, grid_y, grid_z: Grid coordinates
    /// @return World position of block's minimum corner
    glm::vec3 GetBlockWorldPosition(int32_t grid_x, int32_t grid_y, int32_t grid_z) const;

    /// Get AABB for block at grid position
    /// @param grid_x, grid_y, grid_z: Grid coordinates
    /// @return Axis-aligned bounding box for the block
    AABB GetBlockAABB(int32_t grid_x, int32_t grid_y, int32_t grid_z) const;

private:
    // Grid parameters
    uint32_t grid_width_;   // Width in blocks (X axis)
    uint32_t grid_height_;  // Height in blocks (Y axis)
    uint32_t grid_depth_;   // Depth in blocks (Z axis)
    float block_size_;      // Physical size of each block

    // Block storage (linear array indexed as [x + y*width + z*width*height])
    std::vector<Block> blocks_;

    // Visibility state
    ViewFrustum frustum_;
    uint32_t total_blocks_;    // Count of non-air blocks in world
    uint32_t visible_blocks_;  // Count of visible non-air blocks

    /// Convert 3D grid coordinates to linear array index
    /// @param x, y, z: Grid coordinates
    /// @return Linear index or -1 if out of bounds
    int32_t CoordinatesToIndex(int32_t x, int32_t y, int32_t z) const;

    /// Check if grid coordinates are valid
    /// @param x, y, z: Grid coordinates
    /// @return true if coordinates are within grid bounds
    bool IsValidCoordinate(int32_t x, int32_t y, int32_t z) const;

    /// Update total block count (non-air blocks)
    void RecalculateTotalBlockCount();
};

} // namespace world
} // namespace blec

#endif // BLEC_WORLD_BLOCK_SYSTEM_H
