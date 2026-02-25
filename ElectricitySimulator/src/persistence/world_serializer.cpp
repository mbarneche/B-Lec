/**
 * @file world_serializer.cpp
 * @brief World serialization implementation
 */

#include "persistence/world_serializer.h"
#include "world/world.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

bool WorldSerializer::SaveWorld(World* world, const std::string& filepath) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filepath << std::endl;
        return false;
    }

    // Placeholder: Write world data
    // In a real implementation, we would serialize all chunks and blocks

    file.close();
    return true;
}

bool WorldSerializer::LoadWorld(World* world, const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filepath << std::endl;
        return false;
    }

    // Placeholder: Read world data
    // In a real implementation, we would deserialize all chunks and blocks

    file.close();
    return true;
}

bool WorldSerializer::WorldExists(const std::string& filepath) {
    return fs::exists(filepath);
}

std::string WorldSerializer::GetSaveDirectory() {
    std::string save_dir = SAVE_DIRECTORY;
    fs::create_directories(save_dir);
    return save_dir;
}

std::string WorldSerializer::GetWorldPath(const std::string& world_name) {
    std::string save_dir = GetSaveDirectory();
    return save_dir + "/" + world_name + FILE_EXTENSION;
}

bool WorldSerializer::DeleteWorld(const std::string& world_name) {
    std::string path = GetWorldPath(world_name);
    try {
        return fs::remove(path) > 0;
    } catch (const std::exception& e) {
        std::cerr << "Failed to delete world: " << e.what() << std::endl;
        return false;
    }
}
