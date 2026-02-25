/**
 * @file block_selector.cpp
 * @brief Block selector implementation
 */

#include "world/block_selector.h"

BlockSelector::BlockSelector()
    : selected_block_(BlockType::COPPER_WIRE),
      current_hotbar_index_(0),
      menu_visible_(false) {
}

BlockSelector::~BlockSelector() {
}

void BlockSelector::Initialize() {
    // Initialize available blocks
    available_blocks_ = {
        BlockType::AIR,
        BlockType::COPPER_WIRE,
        BlockType::INSULATOR,
        BlockType::POWER_SOURCE,
        BlockType::SWITCH,
        BlockType::BUTTON,
        BlockType::LIGHT,
        BlockType::SENSOR,
        BlockType::REPEATER,
    };

    hotbar_.resize(HOTBAR_SIZE, BlockType::AIR);
    InitializeDefaultHotbar();
    
    selected_block_ = hotbar_[0];
}

void BlockSelector::InitializeDefaultHotbar() {
    if (hotbar_.size() < HOTBAR_SIZE) {
        hotbar_.resize(HOTBAR_SIZE, BlockType::AIR);
    }

    // Set default hotbar layout
    hotbar_[0] = BlockType::COPPER_WIRE;
    hotbar_[1] = BlockType::INSULATOR;
    hotbar_[2] = BlockType::POWER_SOURCE;
    hotbar_[3] = BlockType::SWITCH;
    hotbar_[4] = BlockType::BUTTON;
    hotbar_[5] = BlockType::LIGHT;
    hotbar_[6] = BlockType::SENSOR;
    hotbar_[7] = BlockType::REPEATER;
    hotbar_[8] = BlockType::AIR;
    hotbar_[9] = BlockType::AIR;
}

void BlockSelector::SelectByHotbar(uint32_t index) {
    if (index >= HOTBAR_SIZE) {
        return;
    }

    current_hotbar_index_ = index;
    selected_block_ = hotbar_[index];
}

void BlockSelector::CycleNext() {
    current_hotbar_index_ = (current_hotbar_index_ + 1) % HOTBAR_SIZE;
    selected_block_ = hotbar_[current_hotbar_index_];
}

void BlockSelector::CyclePrevious() {
    if (current_hotbar_index_ == 0) {
        current_hotbar_index_ = HOTBAR_SIZE - 1;
    } else {
        current_hotbar_index_--;
    }
    selected_block_ = hotbar_[current_hotbar_index_];
}

BlockType BlockSelector::GetHotbarBlock(uint32_t index) const {
    if (index >= HOTBAR_SIZE) {
        return BlockType::AIR;
    }
    return hotbar_[index];
}

void BlockSelector::SetHotbarBlock(uint32_t index, BlockType block_type) {
    if (index >= HOTBAR_SIZE) {
        return;
    }
    hotbar_[index] = block_type;
    
    if (index == current_hotbar_index_) {
        selected_block_ = block_type;
    }
}

void BlockSelector::SelectFromMenu(BlockType block_type) {
    selected_block_ = block_type;
    // Optionally hide menu after selection
    menu_visible_ = false;
}
