# World

## Overview
The world module manages blocks, chunks, interaction, selection, raycasting, and power propagation. The world is divided into 16x16x16 chunks for organization.

## Components
- Block/BlockType/BlockRegistry: defines block types and metadata, including colors and descriptions.
- Chunk: stores a fixed-size block array and a dirty flag for mesh regeneration.
- World: owns a map of chunks, provides block access, and runs power propagation at a fixed tick rate.
- BlockInteraction: uses raycasting to place or destroy blocks and toggles switches/buttons.
- BlockSelector: manages the hotbar and available block list.
- Raycast: performs voxel stepping to find the first solid block hit and a placement position.

## Usage
- Call World::Initialize to seed initial chunks and blocks.
- Use GetBlock/SetBlock with world coordinates; conversions to chunk/local coordinates are handled internally.
- Call World::Update each frame to advance power ticks.
- Use BlockSelector for hotbar selection and BlockInteraction for placement/destruction.

## Pitfalls
- World::Initialize fills starter chunks with copper wire for testing.
- Power propagation is binary (powered or not) and uses a BFS over conductors.
- Buttons and repeaters use tick counters, but power level is only 0 or 1.
- Raycast uses fixed step size and does not compute hit face.
- Chunk mesh generation is a placeholder; rendering does not use chunk meshes.
- Block placement/destruction cooldown fields are defined but not enforced.
