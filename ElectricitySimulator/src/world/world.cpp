/**
 * @file world.cpp
 * @brief World implementation
 */

#include "world/world.h"
#include <glm/glm.hpp>

World::World()
    : spawn_point_(0.0f, 32.0f, 0.0f) {
}

World::~World() {
    chunks_.clear();
}

void World::Initialize() {
    // Create initial chunks around the spawn point
    for (int32_t x = -2; x <= 2; ++x) {
        for (int32_t z = -2; z <= 2; ++z) {
            Chunk* chunk = GetOrCreateChunk(x, 0, z);
            // Fill with dirt/stone for testing
            if (chunk) {
                chunk->Fill(BlockType::COPPER_WIRE);
            }
        }
    }
}

Block* World::GetBlock(int32_t world_x, int32_t world_y, int32_t world_z) {
    int32_t chunk_x = WorldToChunkCoord(world_x);
    int32_t chunk_y = WorldToChunkCoord(world_y);
    int32_t chunk_z = WorldToChunkCoord(world_z);

    Chunk* chunk = GetChunk(chunk_x, chunk_y, chunk_z);
    if (!chunk) {
        return nullptr;
    }

    uint32_t local_x = WorldToLocalCoord(world_x);
    uint32_t local_y = WorldToLocalCoord(world_y);
    uint32_t local_z = WorldToLocalCoord(world_z);

    return chunk->GetBlock(local_x, local_y, local_z);
}

const Block* World::GetBlock(int32_t world_x, int32_t world_y, int32_t world_z) const {
    int32_t chunk_x = WorldToChunkCoord(world_x);
    int32_t chunk_y = WorldToChunkCoord(world_y);
    int32_t chunk_z = WorldToChunkCoord(world_z);

    Chunk* chunk = GetChunk(chunk_x, chunk_y, chunk_z);
    if (!chunk) {
        return nullptr;
    }

    uint32_t local_x = WorldToLocalCoord(world_x);
    uint32_t local_y = WorldToLocalCoord(world_y);
    uint32_t local_z = WorldToLocalCoord(world_z);

    return chunk->GetBlock(local_x, local_y, local_z);
}

void World::SetBlock(int32_t world_x, int32_t world_y, int32_t world_z, const Block& block) {
    int32_t chunk_x = WorldToChunkCoord(world_x);
    int32_t chunk_y = WorldToChunkCoord(world_y);
    int32_t chunk_z = WorldToChunkCoord(world_z);

    Chunk* chunk = GetOrCreateChunk(chunk_x, chunk_y, chunk_z);
    if (!chunk) {
        return;
    }

    uint32_t local_x = WorldToLocalCoord(world_x);
    uint32_t local_y = WorldToLocalCoord(world_y);
    uint32_t local_z = WorldToLocalCoord(world_z);

    chunk->SetBlock(local_x, local_y, local_z, block);
}

Chunk* World::GetOrCreateChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z) {
    uint64_t key = GetChunkKey(chunk_x, chunk_y, chunk_z);
    
    auto it = chunks_.find(key);
    if (it != chunks_.end()) {
        return it->second.get();
    }

    auto chunk = std::make_unique<Chunk>(chunk_x, chunk_y, chunk_z);
    Chunk* chunk_ptr = chunk.get();
    chunks_[key] = std::move(chunk);
    
    return chunk_ptr;
}

Chunk* World::GetChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z) {
    uint64_t key = GetChunkKey(chunk_x, chunk_y, chunk_z);
    
    auto it = chunks_.find(key);
    if (it != chunks_.end()) {
        return it->second.get();
    }
    
    return nullptr;
}

std::vector<Chunk*> World::GetAllChunks() {
    std::vector<Chunk*> result;
    for (auto& pair : chunks_) {
        result.push_back(pair.second.get());
    }
    return result;
}

void World::Update() {
    PropagatePower();
}

void World::UnloadDistantChunks(int32_t center_x, int32_t center_y, int32_t center_z, int32_t load_distance) {
    int32_t center_chunk_x = WorldToChunkCoord(center_x);
    int32_t center_chunk_y = WorldToChunkCoord(center_y);
    int32_t center_chunk_z = WorldToChunkCoord(center_z);

    // Unload chunks that are too far away
    std::vector<uint64_t> chunks_to_unload;
    for (auto& pair : chunks_) {
        Chunk* chunk = pair.second.get();
        glm::ivec3 pos = chunk->GetPosition();

        int32_t dx = pos.x - center_chunk_x;
        int32_t dy = pos.y - center_chunk_y;
        int32_t dz = pos.z - center_chunk_z;

        int32_t distance = dx * dx + dy * dy + dz * dz;
        if (distance > load_distance * load_distance) {
            chunks_to_unload.push_back(pair.first);
        }
    }

    for (uint64_t key : chunks_to_unload) {
        chunks_.erase(key);
    }
}

uint64_t World::GetChunkKey(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z) {
    // Create a unique key from the three coordinates
    uint64_t key = 0;
    key |= (static_cast<uint64_t>(chunk_x) & 0x1FFFFF);
    key |= ((static_cast<uint64_t>(chunk_y) & 0x1FF) << 21);
    key |= ((static_cast<uint64_t>(chunk_z) & 0x1FFFFF) << 30);
    return key;
}

void World::PropagatePower() {
    // Placeholder for power propagation system
    // This will be implemented in a later phase
}

void World::UpdateBlockPower(int32_t x, int32_t y, int32_t z) {
    // Placeholder for block power update
    // This will be implemented in a later phase
}
