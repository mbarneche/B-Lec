#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/world/block_interaction.h"
#include "../../ElectricitySimulator/include/world/world.h"
#include "../../ElectricitySimulator/include/camera/camera.h"

TEST(BlockInteractionTest, PlaceAndDestroy) {
    World world;
    world.GetOrCreateChunk(0, 0, 0);

    BlockInteraction interaction;
    interaction.Initialize(6.0f);
    interaction.SetSelectedBlock(BlockType::POWER_SOURCE);

    glm::ivec3 pos(1, 1, 1);
    world.SetBlock(pos.x, pos.y, pos.z, Block(BlockType::AIR));

    EXPECT_TRUE(interaction.IsValidPlacement(&world, pos));
    EXPECT_TRUE(interaction.PlaceBlock(&world, pos));

    Block* placed = world.GetBlock(pos.x, pos.y, pos.z);
    ASSERT_NE(placed, nullptr);
    EXPECT_EQ(placed->type, BlockType::POWER_SOURCE);
    EXPECT_TRUE(placed->is_powered);
    EXPECT_TRUE(placed->is_active);

    EXPECT_TRUE(interaction.CanDestroy(&world, pos));
    EXPECT_TRUE(interaction.DestroyBlock(&world, pos));
    Block* after = world.GetBlock(pos.x, pos.y, pos.z);
    ASSERT_NE(after, nullptr);
    EXPECT_EQ(after->type, BlockType::AIR);
}

TEST(BlockInteractionTest, UpdateSwitchAndButton) {
    World world;
    world.GetOrCreateChunk(0, 0, 0);

    glm::ivec3 switch_pos(0, 0, 1);
    world.SetBlock(switch_pos.x, switch_pos.y, switch_pos.z, Block(BlockType::SWITCH));

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));

    BlockInteraction interaction;
    interaction.Initialize(6.0f);

    interaction.Update(&world, &camera, false, true);
    Block* sw = world.GetBlock(switch_pos.x, switch_pos.y, switch_pos.z);
    ASSERT_NE(sw, nullptr);
    EXPECT_TRUE(sw->is_active);

    world.SetBlock(switch_pos.x, switch_pos.y, switch_pos.z, Block(BlockType::BUTTON));
    interaction.Update(&world, &camera, false, true);
    Block* button = world.GetBlock(switch_pos.x, switch_pos.y, switch_pos.z);
    ASSERT_NE(button, nullptr);
    EXPECT_TRUE(button->is_active);
    EXPECT_GT(button->ticks_remaining, 0);
}
