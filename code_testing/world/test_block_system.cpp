// code_testing/world/test_block_system.cpp
// Unit tests for BlockSystem and frustum culling

#include "../test_framework.h"
#include "world/block_system.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// ============================================================================
// TEST SUITE: Initialization
// ============================================================================

TEST_CASE(TestBlockSystemInitialization) {
    blec::world::BlockSystem system;
    ASSERT_TRUE(system.Initialize(16, 16, 16, 1.0f));
}

TEST_CASE(TestBlockSystemInvalidInitialization) {
    blec::world::BlockSystem system;
    ASSERT_FALSE(system.Initialize(0, 16, 16, 1.0f));
    ASSERT_FALSE(system.Initialize(16, 0, 16, 1.0f));
    ASSERT_FALSE(system.Initialize(16, 16, 0, 1.0f));
    ASSERT_FALSE(system.Initialize(16, 16, 16, -1.0f));
}

// ============================================================================
// TEST SUITE: Block Get/Set
// ============================================================================

TEST_CASE(TestBlockGetSet) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    ASSERT_EQ(system.GetBlock(0, 0, 0).type, 0u);
    ASSERT_TRUE(system.SetBlock(5, 5, 5, blec::world::Block{1}));
    ASSERT_EQ(system.GetBlock(5, 5, 5).type, 1u);
}

TEST_CASE(TestBlockOutOfBounds) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    ASSERT_FALSE(system.SetBlock(20, 0, 0, blec::world::Block{1}));
    ASSERT_EQ(system.GetBlock(20, 0, 0).type, 0u);
    ASSERT_EQ(system.GetBlock(-1, 0, 0).type, 0u);
}

// ============================================================================
// TEST SUITE: Counting and Positions
// ============================================================================

TEST_CASE(TestBlockCounting) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 1.0f);

    ASSERT_EQ(system.GetTotalBlockCount(), 0u);
    system.SetBlock(0, 0, 0, blec::world::Block{1});
    ASSERT_EQ(system.GetTotalBlockCount(), 1u);
    system.SetBlock(1, 0, 0, blec::world::Block{1});
    ASSERT_EQ(system.GetTotalBlockCount(), 2u);
    system.SetBlock(0, 0, 0, blec::world::Block{0});
    ASSERT_EQ(system.GetTotalBlockCount(), 1u);
}

TEST_CASE(TestBlockWorldPosition) {
    blec::world::BlockSystem system;
    system.Initialize(16, 16, 16, 2.0f);

    glm::vec3 pos = system.GetBlockWorldPosition(5, 3, 2);
    ASSERT_TRUE(pos.x == 10.0f && pos.y == 6.0f && pos.z == 4.0f);
}

// ============================================================================
// TEST SUITE: AABB and Frustum
// ============================================================================

TEST_CASE(TestAABBIntersection) {
    blec::world::AABB box1{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    blec::world::AABB box2{{0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 1.5f}};
    blec::world::AABB box3{{2.0f, 2.0f, 2.0f}, {3.0f, 3.0f, 3.0f}};

    ASSERT_TRUE(box1.IntersectsAABB(box2));
    ASSERT_FALSE(box1.IntersectsAABB(box3));
}

TEST_CASE(TestFrustumExtractionAndVisibility) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);
    system.CreateTestBlocks();

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    system.ExtractFrustum(view, projection);
    system.UpdateVisibility();

    ASSERT_TRUE(system.GetVisibleBlockCount() <= system.GetTotalBlockCount());
}

TEST_CASE(TestCreateTestBlocksCount) {
    blec::world::BlockSystem system;
    system.Initialize(32, 32, 32, 1.0f);
    system.CreateTestBlocks();

    ASSERT_EQ(system.GetTotalBlockCount(), 27u);
}

TEST_MAIN()
