/**
 * @file chunk.h
 * @brief Chunk management for world optimization
 * 
 * The world is divided into chunks for efficient rendering and updates.
 * A chunk is a 16x16x16 section of blocks.
 */

#pragma once

#include "world/block.h"
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

constexpr uint32_t CHUNK_SIZE = 16;
constexpr uint32_t CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

/**
 * @class Chunk
 * @brief A 16x16x16 section of the world
 */
class Chunk {
public:
    /**
     * @brief Constructor
     * @param chunk_x X coordinate of the chunk in chunk space
     * @param chunk_y Y coordinate of the chunk in chunk space
     * @param chunk_z Z coordinate of the chunk in chunk space
     */
    Chunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);

    /**
     * @brief Destructor
     */
    ~Chunk();

    /**
     * @brief Get the chunk coordinates
     * @return Chunk position as glm::ivec3
     */
    glm::ivec3 GetPosition() const { return position_; }

    /**
     * @brief Get a block from the chunk
     * @param local_x X coordinate within the chunk (0-15)
     * @param local_y Y coordinate within the chunk (0-15)
     * @param local_z Z coordinate within the chunk (0-15)
     * @return Pointer to the block at that position
     */
    Block* GetBlock(uint32_t local_x, uint32_t local_y, uint32_t local_z);

    /**
     * @brief Get a const block from the chunk
     * @param local_x X coordinate within the chunk (0-15)
     * @param local_y Y coordinate within the chunk (0-15)
     * @param local_z Z coordinate within the chunk (0-15)
     * @return Const pointer to the block at that position
     */
    const Block* GetBlock(uint32_t local_x, uint32_t local_y, uint32_t local_z) const;

    /**
     * @brief Get the block index from local coordinates
     * @param local_x X coordinate within the chunk
     * @param local_y Y coordinate within the chunk
     * @param local_z Z coordinate within the chunk
     * @return Index into the blocks array
     */
    uint32_t GetBlockIndex(uint32_t local_x, uint32_t local_y, uint32_t local_z) const {
        return local_x + (local_y * CHUNK_SIZE) + (local_z * CHUNK_SIZE * CHUNK_SIZE);
    }

    /**
     * @brief Set a block in the chunk
     * @param local_x X coordinate within the chunk
     * @param local_y Y coordinate within the chunk
     * @param local_z Z coordinate within the chunk
     * @param block The block to set
     */
    void SetBlock(uint32_t local_x, uint32_t local_y, uint32_t local_z, const Block& block);

    /**
     * @brief Check if the chunk needs to be remeshed
     * @return true if the chunk's mesh is outdated
     */
    bool IsDirty() const { return is_dirty_; }

    /**
     * @brief Mark the chunk as needing remeshing
     */
    void MarkDirty() { is_dirty_ = true; }

    /**
     * @brief Mark the chunk as up-to-date
     */
    void MarkClean() { is_dirty_ = false; }

    /**
     * @brief Get the mesh data for rendering
     * @return Vector of vertex data
     */
    const std::vector<float>& GetMeshData() const { return mesh_data_; }

    /**
     * @brief Update the mesh based on current block state
     */
    void UpdateMesh();

    /**
     * @brief Fill the chunk with a specific block type
     * @param block_type The block type to fill with
     */
    void Fill(BlockType block_type);

private:
    glm::ivec3 position_;
    std::vector<Block> blocks_;
    std::vector<float> mesh_data_;
    bool is_dirty_;
};
