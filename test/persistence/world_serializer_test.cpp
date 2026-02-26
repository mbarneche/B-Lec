#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/persistence/world_serializer.h"
#include "../../ElectricitySimulator/include/world/world.h"
#include <filesystem>

TEST(WorldSerializerTest, SaveLoadAndDelete) {
    World world;
    std::filesystem::path temp = std::filesystem::temp_directory_path() / "blec_test_world.blec";

    EXPECT_TRUE(WorldSerializer::SaveWorld(&world, temp.string()));
    EXPECT_TRUE(WorldSerializer::WorldExists(temp.string()));
    EXPECT_TRUE(WorldSerializer::LoadWorld(&world, temp.string()));

    std::filesystem::remove(temp);
}

TEST(WorldSerializerTest, SaveDirectoryAndDeleteWorld) {
    std::string path = WorldSerializer::GetWorldPath("unit_test_world");
    World world;

    EXPECT_TRUE(WorldSerializer::SaveWorld(&world, path));
    EXPECT_TRUE(WorldSerializer::WorldExists(path));
    EXPECT_TRUE(WorldSerializer::DeleteWorld("unit_test_world"));
    EXPECT_FALSE(WorldSerializer::WorldExists(path));
}
