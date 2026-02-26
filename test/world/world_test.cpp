#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/world/world.h"

TEST(WorldTest, CoordinateConversionsAndChunks) {
    EXPECT_EQ(World::WorldToChunkCoord(0), 0);
    EXPECT_EQ(World::WorldToChunkCoord(15), 0);
    EXPECT_EQ(World::WorldToChunkCoord(16), 1);
    EXPECT_EQ(World::WorldToChunkCoord(-1), -1);
    EXPECT_EQ(World::WorldToChunkCoord(-16), -1);
    EXPECT_EQ(World::WorldToChunkCoord(-17), -2);

    EXPECT_EQ(World::WorldToLocalCoord(0), 0u);
    EXPECT_EQ(World::WorldToLocalCoord(15), 15u);
    EXPECT_EQ(World::WorldToLocalCoord(16), 0u);
    EXPECT_EQ(World::WorldToLocalCoord(-1), 15u);

    uint64_t key1 = World::GetChunkKey(0, 0, 0);
    uint64_t key2 = World::GetChunkKey(1, 0, 0);
    EXPECT_NE(key1, key2);
}

TEST(WorldTest, SetGetAndUnload) {
    World world;
    world.GetOrCreateChunk(0, 0, 0);
    world.GetOrCreateChunk(10, 0, 0);

    world.SetBlock(0, 0, 0, Block(BlockType::LIGHT));
    Block* block = world.GetBlock(0, 0, 0);
    ASSERT_NE(block, nullptr);
    EXPECT_EQ(block->type, BlockType::LIGHT);

    world.UnloadDistantChunks(0, 0, 0, 1);
    EXPECT_NE(world.GetChunk(0, 0, 0), nullptr);
    EXPECT_EQ(world.GetChunk(10, 0, 0), nullptr);
}

TEST(WorldTest, InitializeAndPowerPropagation) {
    World world;
    world.Initialize();

    Block* spawned = world.GetBlock(0, 0, 0);
    ASSERT_NE(spawned, nullptr);
    EXPECT_EQ(spawned->type, BlockType::COPPER_WIRE);

    world.SetBlock(0, 0, 0, Block(BlockType::POWER_SOURCE));
    world.SetBlock(1, 0, 0, Block(BlockType::COPPER_WIRE));
    world.SetBlock(2, 0, 0, Block(BlockType::LIGHT));

    world.Update(1.0f);

    Block* wire = world.GetBlock(1, 0, 0);
    Block* light = world.GetBlock(2, 0, 0);
    ASSERT_NE(wire, nullptr);
    ASSERT_NE(light, nullptr);
    EXPECT_TRUE(wire->is_powered);
    EXPECT_TRUE(light->is_powered);
}

TEST(WorldTest, ButtonAndRepeaterBehavior) {
    World world;
    world.GetOrCreateChunk(0, 0, 0);

    Block button(BlockType::BUTTON);
    button.ticks_remaining = 1;
    world.SetBlock(0, 0, 0, button);

    Block repeater(BlockType::REPEATER);
    repeater.delay_ticks = 1;
    repeater.is_active = false;
    repeater.ticks_remaining = 0;
    world.SetBlock(1, 0, 0, repeater);

    world.SetBlock(2, 0, 0, Block(BlockType::POWER_SOURCE));

    world.Update(1.0f);

    Block* updated_button = world.GetBlock(0, 0, 0);
    Block* updated_repeater = world.GetBlock(1, 0, 0);
    ASSERT_NE(updated_button, nullptr);
    ASSERT_NE(updated_repeater, nullptr);
    EXPECT_TRUE(updated_button->is_active);
    EXPECT_EQ(updated_button->ticks_remaining, 0);
    EXPECT_TRUE(updated_repeater->is_active);

    world.Update(1.0f);
    updated_button = world.GetBlock(0, 0, 0);
    ASSERT_NE(updated_button, nullptr);
    EXPECT_FALSE(updated_button->is_active);
}
