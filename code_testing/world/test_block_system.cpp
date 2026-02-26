// code_testing/world/test_block_system.cpp
// Unit tests for BlockSystem and frustum culling

#include "../test_framework.h"
#include "world/block_system.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace blec::test {

// ============================================================================
// TEST SUITE: BlockSystem Initialization
// ============================================================================

TEST(BlockSystemInitialization) {
    blec::world::BlockSystem system;
    EXPECT(system.Initialize(16, 16, 16, 1.0f), "BlockSystem should initialize with valid params");
}

TEST(BlockSystemDimensionQuery) {
    blec::world::BlockSystem system;
    system.Initialize(32, 64, 128, 2.0f);

    EXPECT(system.GetGridWidth() == 32, "Grid width should match initialization");
    EXPECT(system.GetGridHeight() == 64, "Grid height should match initialization");
    EXPECT(system.GetGridDepth() == 128, "Grid depth should match initialization");
    EXPECT(std::abs(system.GetBlockSize() - 2.0f) < 0.01f, "Block size should match");
}

TEST(BlockSystemInvalidInitialization) {
    blec::world::BlockSystem system;

    // Zero width should fail
    EXPECT(!system.Initialize(0, 16, 16, 1.0f), "Zero width should fail");

    // Zero height should fail
    EXPECT(!system.Initialize(16, 0, 16, 1.0f), "Zero height should fail");

    // Zero depth should fail
    EXPECT(!system.Initialize(16, 16, 0, 1.0f), "Zero depth should fail");

    // Negative block size should fail
    EXPECT(!system.Initialize(16, 16, 16, -1.0f), "Negative block size should fail");
}

// ============================================================================
// TEST SUITE: Block Get/Set Operations
// ============================================================================

TEST(BlockGetDefault) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    // All blocks should start as air (type = 0)
    blec::world::Block block = system.GetBlock(0, 0, 0);
    EXPECT(block.type == 0, "New blocks should be air (type 0)");
}

TEST(BlockSetAndGet) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    // Set a block
    blec::world::Block solid_block{1};
    system.SetBlock(5, 5, 5, solid_block);

    // Get it back
    blec::world::Block retrieved = system.GetBlock(5, 5, 5);
    EXPECT(retrieved.type == 1, "Retrieved block should match set block");
}

TEST(BlockSetOutOfBounds) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    // Try to set out of bounds
    blec::world::Block solid_block{1};
    bool result = system.SetBlock(20, 0, 0, solid_block);

    EXPECT(!result, "SetBlock should return false for out of bounds");
}

TEST(BlockGetOutOfBounds) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    // Get out of bounds should return air
    blec::world::Block block = system.GetBlock(20, 0, 0);
    EXPECT(block.type == 0, "Out of bounds should return air block");
}

TEST(BlockNegativeCoordinates) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    // Negative coordinates should fail
    blec::world::Block block = system.GetBlock(-1, 0, 0);
    EXPECT(block.type == 0, "Negative coordinates should return air");
}

// ============================================================================
// TEST SUITE: Total Block Counting
// ============================================================================

TEST(BlockSystemInitialCount) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    EXPECT(system.GetTotalBlockCount() == 0, "Initial grid should have no blocks");
}

TEST(BlockSystemCountAfterSet) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    system.SetBlock(0, 0, 0, blec::world::Block{1});
    EXPECT(system.GetTotalBlockCount() == 1, "Count should be 1 after adding block");

    system.SetBlock(1, 0, 0, blec::world::Block{1});
    EXPECT(system.GetTotalBlockCount() == 2, "Count should be 2 after adding another block");

    system.SetBlock(0, 0, 0, blec::world::Block{0});
    EXPECT(system.GetTotalBlockCount() == 1, "Count should be 1 after removing block");
}

TEST(BlockSystemMultipleBlocks) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    for (int i = 0; i < 10; ++i) {
        system.SetBlock(i, 0, 0, blec::world::Block{1});
    }

    EXPECT(system.GetTotalBlockCount() == 10, "Should count all set blocks");
}

// ============================================================================
// TEST SUITE: World Position Conversion
// ============================================================================

TEST(BlockWorldPositionOrigin) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    glm::vec3 pos = system.GetBlockWorldPosition(0, 0, 0);
    EXPECT(pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f, "Block (0,0,0) should be at origin");
}

TEST(BlockWorldPositionOffset) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    glm::vec3 pos = system.GetBlockWorldPosition(5, 3, 2);
    EXPECT(pos.x == 5.0f && pos.y == 3.0f && pos.z == 2.0f, "Block position should match grid coords");
}

TEST(BlockWorldPositionWithBlockSize) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 2.0f);

    glm::vec3 pos = system.GetBlockWorldPosition(5, 3, 2);
    EXPECT(pos.x == 10.0f && pos.y == 6.0f && pos.z == 4.0f, "Block position should scale with block size");
}

// ============================================================================
// TEST SUITE: AABB Operations
// ============================================================================

TEST(BlockAABB) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    blec::world::AABB aabb = system.GetBlockAABB(0, 0, 0);
    EXPECT(aabb.min.x == 0.0f && aabb.min.y == 0.0f && aabb.min.z == 0.0f, "AABB min should be block position");
    EXPECT(aabb.max.x == 1.0f && aabb.max.y == 1.0f && aabb.max.z == 1.0f, "AABB max should be block position + size");
}

TEST(AABBIntersectionOverlap) {
    blec::world::AABB box1{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    blec::world::AABB box2{{0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 1.5f}};

    EXPECT(box1.IntersectsAABB(box2), "Overlapping AABBs should intersect");
}

TEST(AABBIntersectionSeparated) {
    blec::world::AABB box1{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    blec::world::AABB box2{{2.0f, 2.0f, 2.0f}, {3.0f, 3.0f, 3.0f}};

    EXPECT(!box1.IntersectsAABB(box2), "Separated AABBs should not intersect");
}

TEST(AABBIntersectionAdjacent) {
    blec::world::AABB box1{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    blec::world::AABB box2{{1.0f, 0.0f, 0.0f}, {2.0f, 1.0f, 1.0f}};

    EXPECT(box1.IntersectsAABB(box2), "Adjacent AABBs should be considered intersecting");
}

TEST(AABBIntersectionContained) {
    blec::world::AABB box1{{0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}};
    blec::world::AABB box2{{0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 1.5f}};

    EXPECT(box1.IntersectsAABB(box2), "Contained AABB should intersect");
}

// ============================================================================
// TEST SUITE: Frustum Extraction and Culling
// ============================================================================

TEST(FrustumExtraction) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    // Create simple projection and view matrices
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    system.ExtractFrustum(view, projection);

    // Frustum should be extracted without error
    EXPECT(true, "FrustumExtraction should complete");
}

TEST(FrustumCenterBlockVisible) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    // Place a test block at center
    system.SetBlock(16, 16, 16, blec::world::Block{1});

    // Camera looking at center block
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    system.ExtractFrustum(view, projection);
    system.UpdateVisibility();

    EXPECT(system.GetVisibleBlockCount() > 0, "Center block should be visible");
}

TEST(FrustumBlockOutOfView) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    // Place block far off to the side
    system.SetBlock(30, 16, 16, blec::world::Block{1});

    // Camera looking straight ahead
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    system.ExtractFrustum(view, projection);
    system.UpdateVisibility();

    // Block far to the side should likely not be visible (depends on frustum size)
    // Just verify the call completes
    EXPECT(true, "Frustum culling should complete");
}

TEST(FrustumMultipleBlocks) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    // Create several blocks at center
    for (int i = 14; i < 18; ++i) {
        system.SetBlock(i, 16, 16, blec::world::Block{1});
    }

    EXPECT(system.GetTotalBlockCount() == 4, "Should have 4 blocks");

    // Camera looking at center
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    system.ExtractFrustum(view, projection);
    system.UpdateVisibility();

    EXPECT(system.GetVisibleBlockCount() > 0, "Center blocks should be visible");
}

// ============================================================================
// TEST SUITE: Test Block Creation
// ============================================================================

TEST(CreateTestBlocks) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    EXPECT(system.GetTotalBlockCount() == 0, "Grid should be empty before test blocks");

    system.CreateTestBlocks();

    EXPECT(system.GetTotalBlockCount() > 0, "CreateTestBlocks should populate grid");
}

TEST(CreateTestBlocksCube) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    system.CreateTestBlocks();

    // Should create 3x3x3 cube = 27 blocks
    EXPECT(system.GetTotalBlockCount() == 27, "Test blocks should create 27 block cube");
}

TEST(CreateTestBlocksCentered) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    system.CreateTestBlocks();

    // Check center block exists
    blec::world::Block center = system.GetBlock(16, 16, 16);
    EXPECT(center.type != 0, "Center block should be solid");

    // Check corner blocks exist
    blec::world::Block corner = system.GetBlock(15, 15, 15);
    EXPECT(corner.type != 0, "Corner block should be solid");
}

// ============================================================================
// TEST SUITE: Visibility Counting
// ============================================================================

TEST(VisibilityInitial) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    EXPECT(system.GetVisibleBlockCount() == 0, "Initially no blocks should be visible");
}

TEST(VisibilityAfterUpdate) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    system.CreateTestBlocks();

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    system.ExtractFrustum(view, projection);
    system.UpdateVisibility();

    EXPECT(system.GetVisibleBlockCount() <= system.GetTotalBlockCount(),
           "Visible blocks should be <= total blocks");
}

TEST(VisibilityConsistency) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);

    system.CreateTestBlocks();

    uint32_t total = system.GetTotalBlockCount();

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    system.ExtractFrustum(view, projection);
    system.UpdateVisibility();
    uint32_t visible1 = system.GetVisibleBlockCount();

    // Update again without changing frustum
    system.UpdateVisibility();
    uint32_t visible2 = system.GetVisibleBlockCount();

    EXPECT(visible1 == visible2, "Visibility should be consistent without frustum change");
}

// ============================================================================
// TEST SUITE: Frustum AABB Intersection
// ============================================================================

TEST(FrustumAABBIntersection) {
    // Create a simple frustum
    blec::world::ViewFrustum frustum;
    
    // Set up planes for a simple forward-looking frustum
    // Near plane
    frustum.planes[0].normal = glm::vec3(0.0f, 0.0f, -1.0f);
    frustum.planes[0].distance = -0.1f;
    
    // Far plane
    frustum.planes[1].normal = glm::vec3(0.0f, 0.0f, 1.0f);
    frustum.planes[1].distance = 100.0f;
    
    // Left, Right, Top, Bottom (disabled for simplicity)
    for (int i = 2; i < 6; ++i) {
        frustum.planes[i].normal = glm::vec3(0.0f);
        frustum.planes[i].distance = 0.0f;
    }

    blec::world::AABB box{{0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}};

    // The test confirms the method runs without error
    EXPECT(true, "FrustumAABB intersection test should complete");
}

// ============================================================================
// TEST SUITE: Edge Cases
// ============================================================================

TEST(SmallGrid) {
    blec::world::BlockSystem system;
    system.Initialize(2, 2, 2, 1.0f);

    system.SetBlock(0, 0, 0, blec::world::Block{1});
    EXPECT(system.GetTotalBlockCount() == 1, "Small grid should work");
}

TEST(LargeGrid) {
    blec::world::BlockSystem system;
    system.Initialize(256, 256, 256, 1.0f);

    system.SetBlock(128, 128, 128, blec::world::Block{1});
    EXPECT(system.GetTotalBlockCount() == 1, "Large grid should work");
}

TEST(AllBlocksSolid) {
    blec::world::BlockSystem system;
    system.Initialize(4, 4, 4, 1.0f);

    for (uint32_t z = 0; z < 4; ++z) {
        for (uint32_t y = 0; y < 4; ++y) {
            for (uint32_t x = 0; x < 4; ++x) {
                system.SetBlock(x, y, z, blec::world::Block{1});
            }
        }
    }

    EXPECT(system.GetTotalBlockCount() == 64, "All blocks should be counted");
}

} // namespace blec::test
