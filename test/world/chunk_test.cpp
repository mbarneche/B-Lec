#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/world/chunk.h"

TEST(ChunkTest, BasicAccessAndDirtyState) {
    Chunk chunk(1, 2, 3);
    EXPECT_EQ(chunk.GetPosition(), glm::ivec3(1, 2, 3));

    EXPECT_EQ(chunk.GetBlock(CHUNK_SIZE, 0, 0), nullptr);
    EXPECT_EQ(chunk.GetBlock(0, CHUNK_SIZE, 0), nullptr);
    EXPECT_EQ(chunk.GetBlock(0, 0, CHUNK_SIZE), nullptr);

    Block block(BlockType::INSULATOR);
    chunk.SetBlock(0, 0, 0, block);
    Block* placed = chunk.GetBlock(0, 0, 0);
    ASSERT_NE(placed, nullptr);
    EXPECT_EQ(placed->type, BlockType::INSULATOR);

    EXPECT_TRUE(chunk.IsDirty());
    chunk.MarkClean();
    EXPECT_FALSE(chunk.IsDirty());

    chunk.UpdateMesh();
    EXPECT_FALSE(chunk.IsDirty());

    chunk.Fill(BlockType::COPPER_WIRE);
    EXPECT_TRUE(chunk.IsDirty());
}
