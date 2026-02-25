/**
 * @file block.cpp
 * @brief Block implementation
 */

#include "world/block.h"

// Block information table
const BlockInfo BlockRegistry::block_infos_[] = {
    {"Air", "Empty space", false, glm::vec3(0.5f, 0.5f, 0.5f)},                 // AIR
    {"Copper Wire", "Conducts electricity", true, glm::vec3(0.8f, 0.4f, 0.0f)},  // COPPER_WIRE
    {"Insulator", "Blocks electricity", true, glm::vec3(0.2f, 0.2f, 0.2f)},      // INSULATOR
    {"Power Source", "Generates power", true, glm::vec3(1.0f, 1.0f, 0.0f)},      // POWER_SOURCE
    {"Switch", "Can be toggled on/off", true, glm::vec3(0.5f, 0.2f, 0.2f)},      // SWITCH
    {"Button", "Momentary switch", true, glm::vec3(0.6f, 0.3f, 0.3f)},          // BUTTON
    {"Light", "Emits light when powered", true, glm::vec3(1.0f, 1.0f, 0.5f)},    // LIGHT
    {"Sensor", "Detects nearby changes", true, glm::vec3(0.3f, 0.3f, 0.8f)},     // SENSOR
    {"Repeater", "Delays and repeats signal", true, glm::vec3(0.8f, 0.2f, 0.8f)}, // REPEATER
};

const BlockInfo* BlockRegistry::GetBlockInfo(BlockType type) {
    if (type >= BlockType::BLOCK_TYPE_COUNT) {
        return nullptr;
    }
    return &block_infos_[static_cast<uint8_t>(type)];
}

glm::vec3 BlockRegistry::GetBlockColor(BlockType type, bool is_powered) {
    const BlockInfo* info = GetBlockInfo(type);
    if (!info) {
        return glm::vec3(1.0f, 0.0f, 1.0f); // Magenta for error
    }

    glm::vec3 color = info->color;
    
    // Brighten the color if powered
    if (is_powered && type != BlockType::AIR) {
        color = glm::mix(color, glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);
    }

    return color;
}
