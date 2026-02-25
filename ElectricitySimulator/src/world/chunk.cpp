/**
 * @file chunk.cpp
 * @brief Chunk implementation
 */

#include "world/chunk.h"
#include <cstring>

Chunk::Chunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z)
    : position_(chunk_x, chunk_y, chunk_z),
      blocks_(CHUNK_VOLUME, Block(BlockType::AIR)),
      is_dirty_(true) {
}

Chunk::~Chunk() {
}

Block* Chunk::GetBlock(uint32_t local_x, uint32_t local_y, uint32_t local_z) {
    if (local_x >= CHUNK_SIZE || local_y >= CHUNK_SIZE || local_z >= CHUNK_SIZE) {
        return nullptr;
    }
    uint32_t index = GetBlockIndex(local_x, local_y, local_z);
    return &blocks_[index];
}

const Block* Chunk::GetBlock(uint32_t local_x, uint32_t local_y, uint32_t local_z) const {
    if (local_x >= CHUNK_SIZE || local_y >= CHUNK_SIZE || local_z >= CHUNK_SIZE) {
        return nullptr;
    }
    uint32_t index = GetBlockIndex(local_x, local_y, local_z);
    return &blocks_[index];
}

void Chunk::SetBlock(uint32_t local_x, uint32_t local_y, uint32_t local_z, const Block& block) {
    if (local_x >= CHUNK_SIZE || local_y >= CHUNK_SIZE || local_z >= CHUNK_SIZE) {
        return;
    }
    uint32_t index = GetBlockIndex(local_x, local_y, local_z);
    blocks_[index] = block;
    is_dirty_ = true;
}

void Chunk::UpdateMesh() {
    mesh_data_.clear();
    
    // Placeholder mesh generation - we'll make this more sophisticated later
    for (uint32_t x = 0; x < CHUNK_SIZE; ++x) {
        for (uint32_t y = 0; y < CHUNK_SIZE; ++y) {
            for (uint32_t z = 0; z < CHUNK_SIZE; ++z) {
                const Block* block = GetBlock(x, y, z);
                if (block && block->IsSolid()) {
                    // Add vertices for the block
                    // This is simplified - proper implementation would generate cube geometry
                }
            }
        }
    }
    
    is_dirty_ = false;
}

void Chunk::Fill(BlockType block_type) {
    std::fill(blocks_.begin(), blocks_.end(), Block(block_type));
    is_dirty_ = true;
}
