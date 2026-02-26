/**
 * @file world_serializer.cpp
 * @brief World serialization implementation
 */

#include "persistence/world_serializer.h"
#include "world/world.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cctype>

namespace fs = std::filesystem;

bool WorldSerializer::SaveWorld(World* world, const std::string& filepath) {
    // Security: Validate filepath is not empty
    if (filepath.empty()) {
        std::cerr << "Failed to save world: filepath is empty" << std::endl;
        return false;
    }
    
    // Security: Check if world pointer is valid
    if (!world) {
        std::cerr << "Failed to save world: world pointer is null" << std::endl;
        return false;
    }
    
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filepath << std::endl;
        return false;
    }

    // TODO: Implement full world serialization
    // For now, write a basic header and version number
    const uint32_t WORLD_FORMAT_VERSION = 1;
    file.write(reinterpret_cast<const char*>(&WORLD_FORMAT_VERSION), sizeof(uint32_t));
    
    // TODO: Write spawn point
    // TODO: Write all chunks and blocks
    // TODO: Write additional world metadata

    file.close();
    return true;
}

bool WorldSerializer::LoadWorld(World* world, const std::string& filepath) {
    // Security: Validate filepath is not empty
    if (filepath.empty()) {
        std::cerr << "Failed to load world: filepath is empty" << std::endl;
        return false;
    }
    
    // Security: Check if world pointer is valid
    if (!world) {
        std::cerr << "Failed to load world: world pointer is null" << std::endl;
        return false;
    }
    
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filepath << std::endl;
        return false;
    }

    // TODO: Implement full world deserialization
    // For now, read and validate the header
    uint32_t version = 0;
    file.read(reinterpret_cast<char*>(&version), sizeof(uint32_t));
    
    if (file.gcount() != sizeof(uint32_t)) {
        std::cerr << "Failed to read world format version" << std::endl;
        return false;
    }
    
    // Validate version
    if (version != 1) {
        std::cerr << "Unsupported world format version: " << version << std::endl;
        return false;
    }
    
    // TODO: Read spawn point
    // TODO: Read all chunks and blocks
    // TODO: Read additional world metadata

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
    // Security: Validate world_name to prevent path traversal attacks
    // Only allow alphanumeric characters, hyphens, and underscores
    for (char c : world_name) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '_') {
            std::cerr << "Invalid world name: contains illegal characters. "
                      << "Only alphanumeric, hyphens, and underscores allowed." << std::endl;
            return "";
        }
    }
    
    // Additional security: ensure world_name is not empty and not too long
    if (world_name.empty() || world_name.length() > 64) {
        std::cerr << "Invalid world name: must be between 1 and 64 characters." << std::endl;
        return "";
    }
    
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
