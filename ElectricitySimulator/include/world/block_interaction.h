/**
 * @file block_interaction.h
 * @brief Block interaction system
 * 
 * Handles placing and destroying blocks based on player input.
 */

#pragma once

#include <glm/glm.hpp>
#include <cstdint>

class World;
class Camera;
class Block;
enum class BlockType : uint8_t;

/**
 * @class BlockInteraction
 * @brief Manages block placement and destruction
 */
class BlockInteraction {
public:
    /**
     * @brief Constructor
     */
    BlockInteraction();

    /**
     * @brief Destructor
     */
    ~BlockInteraction();

    /**
     * @brief Initialize the interaction system
     * @param max_reach Maximum distance blocks can be placed/destroyed (default 6.0)
     */
    void Initialize(float max_reach = 6.0f);

    /**
     * @brief Update interaction based on camera position
     * @param world The world to interact with
     * @param camera The player's camera
     * @param left_click Whether left mouse button is pressed
     * @param right_click Whether right mouse button is pressed
     */
    void Update(World* world, Camera* camera, bool left_click, bool right_click);

    /**
     * @brief Place a block of the selected type
     * @param world The world
     * @param position World position to place the block
     * @return true if placement was successful
     */
    bool PlaceBlock(World* world, const glm::ivec3& position);

    /**
     * @brief Destroy a block at the given position
     * @param world The world
     * @param position World position of the block to destroy
     * @return true if destruction was successful
     */
    bool DestroyBlock(World* world, const glm::ivec3& position);

    /**
     * @brief Set the currently selected block type
     * @param block_type The block type to select
     */
    void SetSelectedBlock(BlockType block_type);

    /**
     * @brief Get the currently selected block type
     * @return The selected block type
     */
    BlockType GetSelectedBlock() const { return selected_block_; }

    /**
     * @brief Get the maximum reach distance
     * @return Maximum reach in units
     */
    float GetMaxReach() const { return max_reach_; }

    /**
     * @brief Set the maximum reach distance
     * @param distance New maximum reach
     */
    void SetMaxReach(float distance) { max_reach_ = distance; }

    /**
     * @brief Check if a position is valid for block placement
     * @param world The world
     * @param position Position to check
     * @return true if a block can be placed there
     */
    bool IsValidPlacement(World* world, const glm::ivec3& position) const;

    /**
     * @brief Check if a position can be destroyed
     * @param world The world
     * @param position Position to check
     * @return true if the block can be destroyed
     */
    bool CanDestroy(World* world, const glm::ivec3& position) const;

private:
    BlockType selected_block_;
    float max_reach_;
    float last_place_time_;
    float last_destroy_time_;
    float place_cooldown_;      ///< Minimum time between placements (in seconds)
    float destroy_cooldown_;    ///< Minimum time between destructions (in seconds)
};
