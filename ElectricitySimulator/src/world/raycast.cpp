/**
 * @file raycast.cpp
 * @brief Raycasting implementation
 */

#include "world/raycast.h"
#include "world/world.h"
#include "world/block.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Raycast::Raycast() {
}

Raycast::~Raycast() {
}

std::optional<RaycastHit> Raycast::CastRay(
    World* world,
    const glm::vec3& origin,
    const glm::vec3& direction,
    float max_distance) {
    
    if (!world) {
        return std::nullopt;
    }

    // Use DDA (Digital Differential Analyzer) algorithm for voxel traversal
    glm::vec3 current = origin;
    glm::vec3 step(
        direction.x > 0 ? 1.0f : (direction.x < 0 ? -1.0f : 0.0f),
        direction.y > 0 ? 1.0f : (direction.y < 0 ? -1.0f : 0.0f),
        direction.z > 0 ? 1.0f : (direction.z < 0 ? -1.0f : 0.0f)
    );

    glm::vec3 delta(0.0f);
    if (direction.x != 0.0f) delta.x = 1.0f / std::abs(direction.x);
    if (direction.y != 0.0f) delta.y = 1.0f / std::abs(direction.y);
    if (direction.z != 0.0f) delta.z = 1.0f / std::abs(direction.z);

    glm::vec3 next_t(
        delta.x * 0.5f,
        delta.y * 0.5f,
        delta.z * 0.5f
    );

    for (float distance = 0.0f; distance < max_distance; distance += 0.1f) {
        glm::ivec3 block_pos(
            static_cast<int32_t>(std::floor(current.x)),
            static_cast<int32_t>(std::floor(current.y)),
            static_cast<int32_t>(std::floor(current.z))
        );

        Block* block = world->GetBlock(block_pos.x, block_pos.y, block_pos.z);
        
        if (block && block->IsSolid()) {
            // Calculate the adjacent position for placement
            glm::vec3 exit_point = current + (direction * (0.1f - 0.01f));
            glm::ivec3 adjacent_pos(
                static_cast<int32_t>(std::floor(exit_point.x)),
                static_cast<int32_t>(std::floor(exit_point.y)),
                static_cast<int32_t>(std::floor(exit_point.z))
            );

            // Adjust adjacent position if needed
            if (exit_point.x < current.x) adjacent_pos.x -= 1;
            if (exit_point.y < current.y) adjacent_pos.y -= 1;
            if (exit_point.z < current.z) adjacent_pos.z -= 1;

            return RaycastHit{
                block_pos,
                adjacent_pos,
                distance,
                0  // Face calculation would go here
            };
        }

        // Step to next voxel
        if (next_t.x < next_t.y && next_t.x < next_t.z) {
            current.x += step.x;
            next_t.x += delta.x;
        } else if (next_t.y < next_t.z) {
            current.y += step.y;
            next_t.y += delta.y;
        } else {
            current.z += step.z;
            next_t.z += delta.z;
        }
    }

    return std::nullopt;
}

Block* Raycast::GetHitBlock(World* world, const RaycastHit& hit) {
    if (!world) return nullptr;
    return world->GetBlock(hit.block_pos.x, hit.block_pos.y, hit.block_pos.z);
}

Block* Raycast::GetPlacementBlock(World* world, const RaycastHit& hit) {
    if (!world) return nullptr;
    return world->GetBlock(hit.adjacent_pos.x, hit.adjacent_pos.y, hit.adjacent_pos.z);
}

bool Raycast::IsPointInBlock(const glm::ivec3& pos, const glm::vec3& point) {
    return point.x >= pos.x && point.x < pos.x + 1.0f &&
           point.y >= pos.y && point.y < pos.y + 1.0f &&
           point.z >= pos.z && point.z < pos.z + 1.0f;
}
