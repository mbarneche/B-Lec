# World Module

## Purpose
Manages the voxel grid and view-frustum visibility calculations.

## Key Files
- include/world/block_system.h
- src/world/block_system.cpp

## Responsibilities
- Store and query voxel blocks
- Convert grid positions to world-space
- Extract view frustum planes from matrices
- Count visible blocks via frustum culling

## Usage Notes
- `SetBlock()` updates the total count incrementally
- Call `ExtractFrustum()` before `UpdateVisibility()` each frame

## Tests
- code_testing/world/test_block_system.cpp
