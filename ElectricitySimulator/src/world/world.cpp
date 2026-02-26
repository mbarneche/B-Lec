/**
 * @file world.cpp
 * @brief World implementation
 */

#include "world/world.h"
#include <glm/glm.hpp>
#include <queue>
#include <unordered_set>

namespace {
constexpr float kPowerTickRate = 20.0f;
constexpr float kPowerTickInterval = 1.0f / kPowerTickRate;

uint64_t HashBlockPos(int32_t x, int32_t y, int32_t z) {
    uint64_t hx = static_cast<uint32_t>(x);
    uint64_t hy = static_cast<uint32_t>(y);
    uint64_t hz = static_cast<uint32_t>(z);
    uint64_t h = hx;
    h ^= (hy + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2));
    h ^= (hz + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2));
    return h;
}

bool IsPowerSourceBlock(const Block& block) {
    if (block.type == BlockType::POWER_SOURCE) {
        return true;
    }
    if (block.type == BlockType::SWITCH || block.type == BlockType::BUTTON) {
        return block.is_active;
    }
    if (block.type == BlockType::REPEATER) {
        return block.is_active;
    }
    return false;
}

bool IsPowerConsumer(const Block& block) {
    return block.type == BlockType::LIGHT || block.type == BlockType::SENSOR;
}
}

World::World()
    : spawn_point_(0.0f, 32.0f, 0.0f),
      tick_accumulator_(0.0f) {
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

Chunk* World::GetChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z) const {
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

void World::Update(float delta_time) {
    tick_accumulator_ += delta_time;
    while (tick_accumulator_ >= kPowerTickInterval) {
        PropagatePower();
        tick_accumulator_ -= kPowerTickInterval;
    }
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
    // Update button and repeater state based on previous tick
    // This implements a tick-based power system similar to Minecraft redstone
    for (auto& pair : chunks_) {
        Chunk* chunk = pair.second.get();
        glm::ivec3 chunk_pos = chunk->GetPosition();

        for (uint32_t x = 0; x < CHUNK_SIZE; ++x) {
            for (uint32_t y = 0; y < CHUNK_SIZE; ++y) {
                for (uint32_t z = 0; z < CHUNK_SIZE; ++z) {
                    Block* block = chunk->GetBlock(x, y, z);
                    if (!block) {
                        continue;
                    }

                    // Handle button pulse duration
                    // Buttons are active for a limited time then turn off
                    if (block->type == BlockType::BUTTON) {
                        if (block->ticks_remaining > 0) {
                            block->ticks_remaining--;
                            block->is_active = true;
                        } else {
                            block->is_active = false;
                        }
                    }

                    // Handle repeater delay and activation
                    // Repeaters delay signal propagation by a configurable number of ticks
                    if (block->type == BlockType::REPEATER) {
                        // Convert local to world coordinates
                        int32_t world_x = chunk_pos.x * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(x);
                        int32_t world_y = chunk_pos.y * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(y);
                        int32_t world_z = chunk_pos.z * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(z);

                        // Check if any neighboring block is powered
                        bool has_input_power = false;
                        const int32_t neighbors[6][3] = {
                            {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
                        };
                        for (const auto& n : neighbors) {
                            Block* neighbor = GetBlock(world_x + n[0], world_y + n[1], world_z + n[2]);
                            if (!neighbor) {
                                continue;
                            }
                            if (neighbor->is_powered || IsPowerSourceBlock(*neighbor)) {
                                has_input_power = true;
                                break;
                            }
                        }

                        // Update repeater state based on input power
                        if (!has_input_power) {
                            // No input power: turn off repeater
                            block->is_active = false;
                            block->ticks_remaining = 0;
                        } else if (!block->is_active) {
                            // Input power present but repeater not active: count down delay
                            if (block->ticks_remaining == 0) {
                                block->ticks_remaining = block->delay_ticks;
                            }
                            if (block->ticks_remaining > 0) {
                                block->ticks_remaining--;
                                if (block->ticks_remaining == 0) {
                                    block->is_active = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    std::unordered_set<uint64_t> powered;
    std::queue<glm::ivec3> queue;

    for (auto& pair : chunks_) {
        Chunk* chunk = pair.second.get();
        glm::ivec3 chunk_pos = chunk->GetPosition();

        for (uint32_t x = 0; x < CHUNK_SIZE; ++x) {
            for (uint32_t y = 0; y < CHUNK_SIZE; ++y) {
                for (uint32_t z = 0; z < CHUNK_SIZE; ++z) {
                    Block* block = chunk->GetBlock(x, y, z);
                    if (!block) {
                        continue;
                    }

                    if (IsPowerSourceBlock(*block)) {
                        int32_t world_x = chunk_pos.x * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(x);
                        int32_t world_y = chunk_pos.y * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(y);
                        int32_t world_z = chunk_pos.z * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(z);

                        uint64_t key = HashBlockPos(world_x, world_y, world_z);
                        if (powered.insert(key).second) {
                            queue.push(glm::ivec3(world_x, world_y, world_z));
                        }
                    }
                }
            }
        }
    }

    while (!queue.empty()) {
        glm::ivec3 pos = queue.front();
        queue.pop();

        const int32_t neighbors[6][3] = {
            {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
        };
        for (const auto& n : neighbors) {
            int32_t nx = pos.x + n[0];
            int32_t ny = pos.y + n[1];
            int32_t nz = pos.z + n[2];

            Block* neighbor = GetBlock(nx, ny, nz);
            if (!neighbor) {
                continue;
            }

            if (!neighbor->ConductsElectricity()) {
                continue;
            }

            uint64_t key = HashBlockPos(nx, ny, nz);
            if (powered.insert(key).second) {
                queue.push(glm::ivec3(nx, ny, nz));
            }
        }
    }

    for (auto& pair : chunks_) {
        Chunk* chunk = pair.second.get();
        glm::ivec3 chunk_pos = chunk->GetPosition();

        for (uint32_t x = 0; x < CHUNK_SIZE; ++x) {
            for (uint32_t y = 0; y < CHUNK_SIZE; ++y) {
                for (uint32_t z = 0; z < CHUNK_SIZE; ++z) {
                    Block* block = chunk->GetBlock(x, y, z);
                    if (!block) {
                        continue;
                    }

                    int32_t world_x = chunk_pos.x * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(x);
                    int32_t world_y = chunk_pos.y * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(y);
                    int32_t world_z = chunk_pos.z * static_cast<int32_t>(CHUNK_SIZE) + static_cast<int32_t>(z);

                    uint64_t key = HashBlockPos(world_x, world_y, world_z);
                    bool should_power = powered.find(key) != powered.end();

                    if (!should_power && IsPowerConsumer(*block)) {
                        const int32_t neighbors[6][3] = {
                            {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
                        };
                        for (const auto& n : neighbors) {
                            uint64_t neighbor_key = HashBlockPos(world_x + n[0], world_y + n[1], world_z + n[2]);
                            if (powered.find(neighbor_key) != powered.end()) {
                                should_power = true;
                                break;
                            }
                        }
                    }

                    block->is_powered = should_power;
                    block->power_level = should_power ? 1 : 0;
                }
            }
        }
    }
}

void World::UpdateBlockPower([[maybe_unused]] int32_t x, [[maybe_unused]] int32_t y, [[maybe_unused]] int32_t z) {
    // Placeholder for block power update
    // This will be implemented in a later phase
}
