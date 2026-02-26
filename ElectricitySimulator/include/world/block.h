/**
 * @file block.h
 * @brief Block types and properties
 * 
 * Defines the different block types and their properties in the world.
 */

#pragma once

#include <cstdint>
#include <glm/glm.hpp>

/**
 * @enum BlockType
 * @brief Enumeration of all available block types
 */
enum class BlockType : uint8_t {
    AIR = 0,              ///< Non-solid block (empty space)
    COPPER_WIRE = 1,      ///< Conducts electricity
    INSULATOR = 2,        ///< Does not conduct electricity
    POWER_SOURCE = 3,     ///< Generates power (battery)
    SWITCH = 4,           ///< Can be toggled on/off
    BUTTON = 5,           ///< Momentary switch
    LIGHT = 6,            ///< Emits light when powered
    SENSOR = 7,           ///< Detects nearby changes
    REPEATER = 8,         ///< Delays and repeats signal
    
    // Add more block types as needed
    
    BLOCK_TYPE_COUNT      ///< Total number of block types
};

/**
 * @struct Block
 * @brief Represents a single block in the world
 */
struct Block {
    BlockType type;           ///< The type of this block
    bool is_powered;          ///< Whether this block is currently powered
    uint8_t power_level;      ///< Power level (0-15), similar to Minecraft redstone
    uint8_t rotation;         ///< Rotation state (0-3 for block orientation)
    bool is_active;           ///< For switches/buttons/repeaters (player or delayed state)
    uint8_t ticks_remaining;  ///< Countdown for button pulse or repeater delay
    uint8_t delay_ticks;      ///< Repeater delay in ticks
    
    /**
     * @brief Constructor
     * @param block_type Type of the block
     */
    Block(BlockType block_type = BlockType::AIR) 
        : type(block_type),
          is_powered(false),
          power_level(0),
          rotation(0),
          is_active(false),
          ticks_remaining(0),
          delay_ticks(2) {}

    /**
     * @brief Check if this block is solid (not air)
     * @return true if the block is solid
     */
    bool IsSolid() const { return type != BlockType::AIR; }

    /**
     * @brief Check if this block conducts electricity
     * @return true if the block conducts electricity
     */
    bool ConductsElectricity() const { 
        return type == BlockType::COPPER_WIRE || 
               type == BlockType::POWER_SOURCE ||
               type == BlockType::REPEATER;
    }

    /**
     * @brief Check if this block is a power source
     * @return true if the block produces power
     */
    bool IsPowerSource() const { return type == BlockType::POWER_SOURCE; }

    /**
     * @brief Check if this block is a switch
     * @return true if the block is a switch or button
     */
    bool IsSwitch() const { return type == BlockType::SWITCH || type == BlockType::BUTTON; }
};

/**
 * @struct BlockInfo
 * @brief Static information about a block type
 */
struct BlockInfo {
    const char* name;           ///< Display name of the block
    const char* description;    ///< Description of the block
    bool is_solid;              ///< Whether the block is solid
    glm::vec3 color;            ///< Base color of the block
};

/**
 * @class BlockRegistry
 * @brief Registry for block types and their properties
 */
class BlockRegistry {
public:
    /**
     * @brief Get information about a block type
     * @param type The block type
     * @return Pointer to BlockInfo for the type
     */
    static const BlockInfo* GetBlockInfo(BlockType type);

    /**
     * @brief Get the color of a block type
     * @param type The block type
     * @param is_powered Whether the block is currently powered (affects color)
     * @return The color of the block
     */
    static glm::vec3 GetBlockColor(BlockType type, bool is_powered);

private:
    static const BlockInfo block_infos_[];
};
