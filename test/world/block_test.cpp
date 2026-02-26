#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/world/block.h"

TEST(BlockTest, DefaultsAndQueries) {
    Block block;
    EXPECT_EQ(block.type, BlockType::AIR);
    EXPECT_FALSE(block.is_powered);
    EXPECT_EQ(block.power_level, 0);
    EXPECT_FALSE(block.IsSolid());

    Block wire(BlockType::COPPER_WIRE);
    EXPECT_TRUE(wire.IsSolid());
    EXPECT_TRUE(wire.ConductsElectricity());
    EXPECT_FALSE(wire.IsPowerSource());
    EXPECT_FALSE(wire.IsSwitch());

    Block power(BlockType::POWER_SOURCE);
    EXPECT_TRUE(power.IsPowerSource());
    EXPECT_TRUE(power.ConductsElectricity());

    Block button(BlockType::BUTTON);
    EXPECT_TRUE(button.IsSwitch());
}

TEST(BlockTest, BlockRegistryInfoAndColor) {
    const BlockInfo* info = BlockRegistry::GetBlockInfo(BlockType::COPPER_WIRE);
    ASSERT_NE(info, nullptr);
    EXPECT_STREQ(info->name, "Copper Wire");

    const BlockInfo* invalid = BlockRegistry::GetBlockInfo(static_cast<BlockType>(255));
    EXPECT_EQ(invalid, nullptr);

    glm::vec3 base = BlockRegistry::GetBlockColor(BlockType::COPPER_WIRE, false);
    glm::vec3 powered = BlockRegistry::GetBlockColor(BlockType::COPPER_WIRE, true);
    EXPECT_GT(powered.r, base.r);

    glm::vec3 error_color = BlockRegistry::GetBlockColor(static_cast<BlockType>(255), false);
    EXPECT_FLOAT_EQ(error_color.r, 1.0f);
    EXPECT_FLOAT_EQ(error_color.g, 0.0f);
    EXPECT_FLOAT_EQ(error_color.b, 1.0f);
}
