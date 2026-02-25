/**
 * @file block_interaction.cpp
 * @brief Block interaction implementation
 */

#include "world/block_interaction.h"
#include "world/world.h"
#include "world/raycast.h"
#include "camera/camera.h"

BlockInteraction::BlockInteraction()
    : selected_block_(BlockType::COPPER_WIRE),
      max_reach_(6.0f),
      last_place_time_(0.0f),
      last_destroy_time_(0.0f),
      place_cooldown_(0.1f),
      destroy_cooldown_(0.1f) {
}

BlockInteraction::~BlockInteraction() {
}

void BlockInteraction::Initialize(float max_reach) {
    max_reach_ = max_reach;
}

void BlockInteraction::Update(World* world, Camera* camera, bool left_click, bool right_click) {
    if (!world || !camera) {
        return;
    }

    // Perform raycast from camera position
    Raycast raycast;
    std::optional<RaycastHit> hit = raycast.CastRay(
        world,
        camera->GetPosition(),
        camera->GetForward(),
        max_reach_
    );

    // Right click to place block
    if (right_click && hit) {
        PlaceBlock(world, hit->adjacent_pos);
    }

    // Left click to destroy block
    if (left_click && hit) {
        DestroyBlock(world, hit->block_pos);
    }
}

bool BlockInteraction::PlaceBlock(World* world, const glm::ivec3& position) {
    if (!IsValidPlacement(world, position)) {
        return false;
    }

    Block block(selected_block_);
    block.is_powered = (selected_block_ == BlockType::POWER_SOURCE);
    
    world->SetBlock(position.x, position.y, position.z, block);
    return true;
}

bool BlockInteraction::DestroyBlock(World* world, const glm::ivec3& position) {
    if (!CanDestroy(world, position)) {
        return false;
    }

    Block air_block(BlockType::AIR);
    world->SetBlock(position.x, position.y, position.z, air_block);
    return true;
}

void BlockInteraction::SetSelectedBlock(BlockType block_type) {
    selected_block_ = block_type;
}

bool BlockInteraction::IsValidPlacement(World* world, const glm::ivec3& position) const {
    if (!world) {
        return false;
    }

    Block* block = world->GetBlock(position.x, position.y, position.z);
    if (!block) {
        return false;
    }

    // Can only place on air blocks
    return block->type == BlockType::AIR;
}

bool BlockInteraction::CanDestroy(World* world, const glm::ivec3& position) const {
    if (!world) {
        return false;
    }

    Block* block = world->GetBlock(position.x, position.y, position.z);
    if (!block) {
        return false;
    }

    // Can't destroy air blocks, and with expansion can add other restrictions
    return block->type != BlockType::AIR;
}
