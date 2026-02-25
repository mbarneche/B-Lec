/**
 * @file block_selector.h
 * @brief Block selection UI and logic
 * 
 * Manages the block hotbar and selection menu.
 */

#pragma once

#include "world/block.h"
#include <vector>
#include <cstdint>

/**
 * @class BlockSelector
 * @brief Manages block selection with hotbar and menu
 */
class BlockSelector {
public:
    static constexpr uint32_t HOTBAR_SIZE = 10;  ///< Number of quick-select slots

    /**
     * @brief Constructor
     */
    BlockSelector();

    /**
     * @brief Destructor
     */
    ~BlockSelector();

    /**
     * @brief Initialize the selector
     */
    void Initialize();

    /**
     * @brief Get the currently selected block type
     * @return The selected block type
     */
    BlockType GetSelectedBlock() const { return selected_block_; }

    /**
     * @brief Set the selected block by hotbar index
     * @param index Index (0-9)
     */
    void SelectByHotbar(uint32_t index);

    /**
     * @brief Cycle to the next block in the hotbar
     */
    void CycleNext();

    /**
     * @brief Cycle to the previous block in the hotbar
     */
    void CyclePrevious();

    /**
     * @brief Get the current hotbar index
     * @return Index of selected hotbar slot
     */
    uint32_t GetCurrentHotbarIndex() const { return current_hotbar_index_; }

    /**
     * @brief Get the block at a hotbar slot
     * @param index Hotbar index
     * @return BlockType at that slot
     */
    BlockType GetHotbarBlock(uint32_t index) const;

    /**
     * @brief Set the block at a hotbar slot
     * @param index Hotbar index
     * @param block_type Block type to place there
     */
    void SetHotbarBlock(uint32_t index, BlockType block_type);

    /**
     * @brief Get all block types available for selection
     * @return Vector of all available block types
     */
    const std::vector<BlockType>& GetAvailableBlocks() const { return available_blocks_; }

    /**
     * @brief Toggle the block selection menu
     * @param visible true to show the menu
     */
    void SetMenuVisible(bool visible) { menu_visible_ = visible; }

    /**
     * @brief Check if the selection menu is visible
     * @return true if menu is shown
     */
    bool IsMenuVisible() const { return menu_visible_; }

    /**
     * @brief Select a block from the menu
     * @param block_type The block to select
     */
    void SelectFromMenu(BlockType block_type);

private:
    /**
     * @brief Initialize the default hotbar layout
     */
    void InitializeDefaultHotbar();

private:
    BlockType selected_block_;
    uint32_t current_hotbar_index_;
    std::vector<BlockType> hotbar_;
    std::vector<BlockType> available_blocks_;
    bool menu_visible_;
};
