/**
 * @file world.h
 * @brief World management and chunk handling
 * 
 * The World class manages all chunks, blocks, and the overall game world state.
 */

#pragma once

#include "world/chunk.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include <vector>

/**
 * @class World
 * @brief Manages the game world, chunks, and block interactions
 */
class World {
public:
    /**
     * @brief Constructor
     */
    World();

    /**
     * @brief Destructor
     */
    ~World();

    /**
     * @brief Initialize the world with default blocks
     */
    void Initialize();

    /**
     * @brief Get a block at world coordinates
     * @param world_x World X coordinate
     * @param world_y World Y coordinate
     * @param world_z World Z coordinate
     * @return Pointer to the block, or nullptr if out of bounds
     */
    Block* GetBlock(int32_t world_x, int32_t world_y, int32_t world_z);

    /**
     * @brief Get a const block at world coordinates
     * @param world_x World X coordinate
     * @param world_y World Y coordinate
     * @param world_z World Z coordinate
     * @return Const pointer to the block, or nullptr if out of bounds
     */
    const Block* GetBlock(int32_t world_x, int32_t world_y, int32_t world_z) const;

    /**
     * @brief Set a block at world coordinates
     * @param world_x World X coordinate
     * @param world_y World Y coordinate
     * @param world_z World Z coordinate
     * @param block The block to place
     */
    void SetBlock(int32_t world_x, int32_t world_y, int32_t world_z, const Block& block);

    /**
     * @brief Get or create a chunk at chunk coordinates
     * @param chunk_x Chunk X coordinate
     * @param chunk_y Chunk Y coordinate
     * @param chunk_z Chunk Z coordinate
     * @return Pointer to the chunk
     */
    Chunk* GetOrCreateChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);

    /**
     * @brief Get a chunk at chunk coordinates
     * @param chunk_x Chunk X coordinate
     * @param chunk_y Chunk Y coordinate
     * @param chunk_z Chunk Z coordinate
     * @return Pointer to the chunk, or nullptr if it doesn't exist
     */
    Chunk* GetChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z) const;

    /**
     * @brief Get all loaded chunks
     * @return Vector of pointers to all chunks
     */
    std::vector<Chunk*> GetAllChunks();

    /**
     * @brief Update the world state (power propagation, etc.)
     * @param delta_time Time elapsed since last frame in seconds
     */
    void Update(float delta_time);

    /**
     * @brief Unload chunks that are far from the given position
     * @param center_x Center X coordinate
     * @param center_y Center Y coordinate
     * @param center_z Center Z coordinate
     * @param load_distance Maximum distance to keep chunks loaded
     */
    void UnloadDistantChunks(int32_t center_x, int32_t center_y, int32_t center_z, int32_t load_distance);

    /**
     * @brief Get the spawn point of the world
     * @return World position to spawn at
     */
    glm::vec3 GetSpawnPoint() const { return spawn_point_; }

    /**
     * @brief Convert world coordinates to chunk coordinates
     * @param world_coord World coordinate value
     * @return Chunk coordinate
     */
    static int32_t WorldToChunkCoord(int32_t world_coord) {
        return world_coord >= 0 ? world_coord / CHUNK_SIZE : (world_coord / CHUNK_SIZE) - 1;
    }

    /**
     * @brief Convert world coordinates to local chunk coordinates
     * @param world_coord World coordinate value
     * @return Local coordinate (0-15)
     */
    static uint32_t WorldToLocalCoord(int32_t world_coord) {
        int32_t chunk_coord = WorldToChunkCoord(world_coord);
        return world_coord - (chunk_coord * CHUNK_SIZE);
    }

    /**
     * @brief Generate a hash key for chunk coordinates
     * @param chunk_x Chunk X coordinate
     * @param chunk_y Chunk Y coordinate
     * @param chunk_z Chunk Z coordinate
     * @return Hash key
     */
    static uint64_t GetChunkKey(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);

private:
    /**
     * @brief Propagate power through the world
     */
    void PropagatePower();

    /**
     * @brief Update power for a single block
     * @param x World X coordinate
     * @param y World Y coordinate
     * @param z World Z coordinate
     */
    void UpdateBlockPower(int32_t x, int32_t y, int32_t z);

private:
    std::unordered_map<uint64_t, std::unique_ptr<Chunk>> chunks_;
    glm::vec3 spawn_point_;
    float tick_accumulator_;
    
    // Tracking which chunks need power updates
    std::vector<glm::ivec3> power_update_queue_;
};
