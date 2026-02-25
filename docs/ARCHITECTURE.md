# B-Lec Internal Architecture

This document describes the internal architecture and design decisions of the B-Lec electricity simulator.

## Overview

B-Lec is organized using a **System-Based Architecture** where distinct functionalities are separated into independent systems that communicate through well-defined interfaces:

```
┌─────────────────────────────────────────────┐
│         Application (Main Loop)              │
├─────────────────────────────────────────────┤
│                                               │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐   │
│  │ Renderer │  │  Camera  │  │  Input   │   │
│  │ System   │  │ System   │  │ Manager  │   │
│  └──────────┘  └──────────┘  └──────────┘   │
│                                               │
│  ┌──────────────────────────────────────┐   │
│  │          World System                 │   │
│  │  ┌──────────┐  ┌──────────────────┐  │   │
│  │  │ Chunks   │  │ Power Simulation │  │   │
│  │  └──────────┘  └──────────────────┘  │   │
│  └──────────────────────────────────────┘   │
│                                               │
│  ┌──────────┐  ┌──────────────────────┐   │
│  │ UI       │  │ World Serializer    │   │
│  │ System   │  │ (Save/Load)         │   │
│  └──────────┘  └──────────────────────┘   │
│                                               │
└─────────────────────────────────────────────┘
```

## Systems

### Application System (`core/`)
- **Class**: `Application`
- **Responsibilities**:
  - Main game loop
  - System initialization and cleanup
  - Timing and delta time calculation
  - Event dispatching

### Rendering System (`rendering/`)
- **Classes**:
  - `Renderer`: Main rendering engine using OpenGL 4.6
  - `Shader`: GLSL shader compilation and management
- **Responsibilities**:
  - 3D rendering with OpenGL
  - Vertex buffer management (VAO/VBO/EBO)
  - Scene graph rendering
  - View and projection matrix management

### World System (`world/`)
- **Classes**:
  - `World`: Main world container and manager
  - `Chunk`: 16x16x16 cubic sections of the world
  - `Block`: Individual block data structure
  - `BlockRegistry`: Static block type information
- **Responsibilities**:
  - Block and chunk management
  - Infinite world support
  - Power propagation simulation
  - Block interaction handling
  - Chunk loading/unloading

### Camera System (`camera/`)
- **Class**: `Camera`
- **Responsibilities**:
  - 3D camera management
  - View and projection matrices
  - Camera movement and rotation

### Input System (`input/`)
- **Class**: `InputManager`
- **Responsibilities**:
  - Keyboard input handling
  - Mouse input tracking
  - Input state tracking (pressed vs just pressed)

### UI System (`ui/`)
- **Class**: `UISystem`
- **Responsibilities**:
  - On-screen HUD rendering
  - Block selection UI
  - Debug overlay
  - Status message display

### Persistence System (`persistence/`)
- **Class**: `WorldSerializer`
- **Responsibilities**:
  - World saving and loading
  - File I/O
  - Data serialization/deserialization

## Data Structures

### Block
```cpp
struct Block {
    BlockType type;        // Type of block
    bool is_powered;       // Current power state
    uint8_t power_level;   // Power level (0-15)
    uint8_t rotation;      // Block orientation
};
```

### Chunk
- Stores a 16×16×16 grid of blocks (4,096 blocks per chunk)
- Tracks dirty state for mesh updates
- Stores mesh data for efficient rendering

### World
- Manages chunks using a 3D hash map
- Uses chunk coordinates for efficient lookup
- Implements infinite world generation

## Coordinate Systems

### World Coordinates
- Global 3D coordinates in world space
- Range: -∞ to +∞ for X, Y, Z

### Chunk Coordinates
- Derived from world coordinates
- Formula: `chunk_coord = world_coord / CHUNK_SIZE` (with proper floor division)
- Enables efficient chunk management

### Local Coordinates
- Coordinates within a chunk (0-15 for each axis)
- Formula: `local_coord = world_coord % CHUNK_SIZE`

## Block Types

Currently implemented block types:
- **AIR** (0): Empty space (non-solid)
- **COPPER_WIRE** (1): Conducts electricity
- **INSULATOR** (2): Blocks conductor connections
- **POWER_SOURCE** (3): Generates continuous power
- **SWITCH** (4): Can be toggled on/off
- **BUTTON** (5): Momentary/pulsed power source
- **LIGHT** (6): Displays power visually
- **SENSOR** (7): Detects nearby changes
- **REPEATER** (8): Delays signal propagation

Additional block types can be added to the `BlockType` enum and registered in `BlockRegistry`.

## Power System

The power system (work in progress):
- Power propagates between adjacent blocks
- Power levels range from 0-15 (similar to Minecraft redstone)
- Each block type reacts differently to power
- Simulation runs once per frame through `World::Update()`

## Memory Management

- Uses `std::unique_ptr` for automatic resource cleanup
- Chunks are dynamically allocated and can be unloaded
- No manual `delete` calls required

## Dependencies

### External Libraries
- **GLFW**: Window and input management
- **OpenGL**: Rendering (via GLAD)
- **GLM**: Vector and matrix mathematics

### Standard Library Only
- Most code uses standard C++ 17 features
- Minimal external dependencies for maintainability

## Future Architecture Considerations

1. **Entity-Component-System (ECS)**: May transition for more flexible entity management
2. **Event System**: Central event queue for inter-system communication
3. **Threading**: Chunk generation/loading on separate threads
4. **Network Architecture**: For future multiplayer support

## Thread Safety

Currently **not thread-safe**. All interactions with World, Renderer, and Camera must occur on the main thread. Future multithreading support will require:
- Chunk generation on worker threads
- Proper synchronization primitives
- Thread-safe command queues
