#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/world/block_selector.h"

TEST(BlockSelectorTest, InitializeAndSelection) {
    BlockSelector selector;
    selector.Initialize();

    EXPECT_EQ(selector.GetCurrentHotbarIndex(), 0u);
    EXPECT_EQ(selector.GetSelectedBlock(), BlockType::COPPER_WIRE);

    selector.SelectByHotbar(1);
    EXPECT_EQ(selector.GetCurrentHotbarIndex(), 1u);
    EXPECT_EQ(selector.GetSelectedBlock(), selector.GetHotbarBlock(1));

    selector.CycleNext();
    EXPECT_EQ(selector.GetCurrentHotbarIndex(), 2u);

    selector.CyclePrevious();
    EXPECT_EQ(selector.GetCurrentHotbarIndex(), 1u);

    selector.SetMenuVisible(true);
    EXPECT_TRUE(selector.IsMenuVisible());

    selector.SelectFromMenu(BlockType::LIGHT);
    EXPECT_EQ(selector.GetSelectedBlock(), BlockType::LIGHT);
    EXPECT_FALSE(selector.IsMenuVisible());
}

TEST(BlockSelectorTest, HotbarSetAndBounds) {
    BlockSelector selector;
    selector.Initialize();

    selector.SetHotbarBlock(0, BlockType::SENSOR);
    EXPECT_EQ(selector.GetSelectedBlock(), BlockType::SENSOR);

    selector.SetHotbarBlock(42, BlockType::POWER_SOURCE);
    EXPECT_EQ(selector.GetHotbarBlock(42), BlockType::AIR);

    selector.SelectByHotbar(42);
    EXPECT_EQ(selector.GetCurrentHotbarIndex(), 0u);
}
