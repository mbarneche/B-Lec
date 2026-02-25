/**
 * @file raycast.h
 * @brief Raycasting system for block selection
 * 
 * Handles raycasts for accurate block detection and selection.
 */

#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <cstdint>

class World;
class Block;

/**
 * @struct RaycastHit
 * @brief Result of a raycast operation
 */
struct RaycastHit {
    glm::ivec3 block_pos;           ///< Position of the hit block
    glm::ivec3 adjacent_pos;        ///< Position where a new block would be placed
    float distance;                 ///< Distance from ray origin to hit
    int32_t face;                   ///< Which face was hit (0-5)
};

/**
 * @class Raycast
 * @brief Performs raycasting operations on the world
 */
class Raycast {
public:
    /**
     * @brief Constructor
     */
    Raycast();

    /**
     * @brief Destructor
     */
    ~Raycast();

    /**
     * @brief Cast a ray into the world
     * @param world The world to cast into
     * @param origin Ray origin point
     * @param direction Ray direction (should be normalized)
     * @param max_distance Maximum distance to check
     * @return RaycastHit if a block was hit, nullopt otherwise
     */
    std::optional<RaycastHit> CastRay(
        World* world,
        const glm::vec3& origin,
        const glm::vec3& direction,
        float max_distance = 100.0f
    );

    /**
     * @brief Get the hit block for a raycast
     * @param world The world
     * @param hit The raycast hit data
     * @return Pointer to the hit block
     */
    static Block* GetHitBlock(World* world, const RaycastHit& hit);

    /**
     * @brief Get the block to place in
     * @param world The world
     * @param hit The raycast hit data
     * @return Pointer to the block position where new blocks would be placed
     */
    static Block* GetPlacementBlock(World* world, const RaycastHit& hit);

private:
    /**
     * @brief Check if a point is inside a block
     * @param pos Block position
     * @param point Test point
     * @return true if the point is inside the block bounds
     */
    static bool IsPointInBlock(const glm::ivec3& pos, const glm::vec3& point);
};
