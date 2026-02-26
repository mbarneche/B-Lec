#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/world/raycast.h"
#include "../../ElectricitySimulator/include/world/world.h"

TEST(RaycastTest, NullWorld) {
    Raycast raycast;
    auto hit = raycast.CastRay(nullptr, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    EXPECT_FALSE(hit.has_value());
}

TEST(RaycastTest, HitBlockAndHelpers) {
    World world;
    world.GetOrCreateChunk(0, 0, 0);
    world.SetBlock(0, 0, 1, Block(BlockType::COPPER_WIRE));

    Raycast raycast;
    auto hit = raycast.CastRay(&world, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 1.0f), 5.0f);
    ASSERT_TRUE(hit.has_value());
    EXPECT_EQ(hit->block_pos, glm::ivec3(0, 0, 1));

    Block* hit_block = Raycast::GetHitBlock(&world, *hit);
    ASSERT_NE(hit_block, nullptr);
    EXPECT_EQ(hit_block->type, BlockType::COPPER_WIRE);

    Block* place_block = Raycast::GetPlacementBlock(&world, *hit);
    ASSERT_NE(place_block, nullptr);
}
