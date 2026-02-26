# B-Lec Code Architecture

## Overview

B-Lec follows a **modular architecture** where functionality is separated into independent, loosely-coupled modules. Each module has a specific responsibility and communicates through well-defined interfaces.

## Design Principles

### 1. Separation of Concerns
Each module handles a single aspect of the application:
- **Window**: Window lifecycle and OS integration
- **Input**: Keyboard and mouse event handling
- **Render**: Graphics rendering and text display
- **Debug**: Development tools and overlay

### 2. Minimal Dependencies
- Only GLFW (windowing) and OpenGL (rendering)
- No external testing frameworks (custom lightweight framework)
- Standard C++17 library only

### 3. Cross-Platform Compatibility
- GLFW for cross-platform window management
- OpenGL 2.1 for maximum hardware compatibility
- Standard C++ without platform-specific extensions

### 4. Privacy and Security
- No data collection or telemetry
- No network communication
- All data stored locally
- No external resource access

## Module Architecture

```
┌─────────────────────────────────────────┐
│              main.cpp                   │
│         (Application Entry)             │
└────────────┬────────────────────────────┘
             │
             │ Coordinates
             ▼
┌────────────────────────────────────────────┐
│  ┌────────────┐  ┌────────────┐           │
│  │  Window    │  │   Input    │           │
│  │  Manager   │◄─┤  Handler   │           │
│  └────────────┘  └────────────┘           │
│        │               │                   │
│        ▼               ▼                   │
│  ┌────────────────────────────┐           │
│  │      Renderer               │           │
│  │  ┌──────────┐ ┌──────────┐ │           │
│  │  │  Camera  │ │  Mesh    │ │           │
│  │  │  Font    │ │ OpenGL   │ │           │
│  │  └──────────┘ └──────────┘ │           │
│  └────────────────────────────┘           │
│                │                           │
│                ▼                           │
│  ┌────────────────────────────┐           │
│  │    Block System            │           │
│  │  (Voxel Grid + Frustum)    │           │
│  └────────────────────────────┘           │
│                │                           │
│                ▼                           │
│  ┌────────────────────────────┐           │
│  │    UI Manager              │           │
│  │  (Crosshair + Pause Menu)  │           │
│  └────────────────────────────┘           │
│                │                           │
│                ▼                           │
│  ┌────────────────────────────┐           │
│  │    Debug Overlay           │           │
│  └────────────────────────────┘           │
└────────────────────────────────────────────┘
```

## Module Details

### Window Module (`include/window/`, `src/window/`)

**Purpose**: Manages the application window and GLFW lifecycle.

**Key Classes**:
- `WindowManager`: Handles window creation, events, and cleanup

**Key Features**:
- GLFW initialization and termination
- Window creation with configurable size
- Framebuffer size queries
- VSync control
- Close state management

**Dependencies**:
- GLFW
- OpenGL (implicit through GLFW)

### Input Module (`include/input/`, `src/input/`)

**Purpose**: Tracks keyboard and mouse input state.

**Key Classes**:
- `InputHandler`: Manages input callbacks and state queries

**Key Features**:
- Keyboard press/release tracking
- Mouse position and delta tracking
- Key name and action string conversion
- Per-frame delta reset
- GLFW callback registration

**Dependencies**:
- GLFW (for callbacks and key constants)

### Render Module (`include/render/`, `src/render/`)

**Purpose**: Provides rendering operations for 2D and 3D graphics.

**Key Classes**:
- `Renderer`: OpenGL rendering operations (2D and 3D)
- `BitmapFont`: 5x7 pixel font for text rendering
- `Camera`: Free-flying 3D camera with WASD movement and mouse look
- `Mesh`: 3D geometry with vertex data and rendering

**Key Features**:
- 2D screen-space rendering and text display
- 3D perspective projection and view matrices
- Free-flying camera with WASD movement and mouse look rotation
- Colored cube mesh with per-face colors
- Back-face culling for optimization
- Depth testing for proper 3D rendering
- Color, blending, and line width control

**3D Components**:
- `Camera` class manages position, orientation (yaw/pitch), and view matrix
- `Mesh` class provides vertex/index data and rendering
- `Renderer` extends with `Begin3D`/`End3D` for 3D mode setup
- Uses GLM library for vector and matrix mathematics
- OpenGL immediate mode for OpenGL 2.1 compatibility

**Dependencies**:
- OpenGL 2.1 (via GLFW)
- GLM (header-only math library)

**Font Data**:
- Public domain 5x7 pixel bitmap font
- Supports ASCII 32-126 (printable characters)
- Scalable rendering

### Camera Module (part of Render, `include/render/camera.h`, `src/render/camera.cpp`)

**Purpose**: Manages a free-flying 3D camera for scene visualization.

**Key Classes**:
- `Camera`: Implements FPS-style camera with WASD movement and mouse look

**Key Features**:
- Position tracking with x/y/z coordinates
- Euler angle rotation (yaw for horizontal, pitch for vertical)
- Camera basis vectors (forward, right, up) derived from rotation angles
- Movement along camera basis directions (forward/backward, strafe, up/down)
- Configurable movement and rotation speeds
- View matrix generation for rendering
- Pitch clamp to prevent gimbal lock (±89.9°)

**Public Methods**:
- `Initialize(position, target)`: Set initial camera pose
- `MoveForward(distance)`: Move along forward direction
- `MoveRight(distance)`: Move along right direction
- `MoveUp(distance)`: Move along up direction
- `Yaw(radians)`: Rotate around world up axis
- `Pitch(radians)`: Rotate around camera right axis
- `Update(delta_time)`: Update internal state
- `GetViewMatrix()`: Get glm::mat4 view matrix for rendering
- `GetPosition()`: Get current position
- `GetForward()/GetRight()/GetUp()`: Get camera basis vectors
- `SetMovementSpeed(speed)`, `SetRotationSpeed(speed)`: Configure speeds

**Typical Usage**:
```cpp
blec::render::Camera camera;
camera.Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
camera.SetMovementSpeed(5.0f);
camera.SetRotationSpeed(0.005f);

// In main loop:
if (input_handler.IsKeyDown(GLFW_KEY_W)) camera.MoveForward(delta_time * 5.0);
auto [dx, dy] = input_handler.GetMouseLookDelta();
camera.Yaw(dx * 0.005f);
camera.Pitch(-dy * 0.005f);
camera.Update(delta_time);

glm::mat4 view = camera.GetViewMatrix();
renderer.SetView(view);
```

### Mesh Module (part of Render, `include/render/mesh.h`, `src/render/mesh.cpp`)

**Purpose**: Manages 3D geometry definition and rendering with culling support.

**Key Structures**:
- `Vertex`: Combines position (vec3), color (vec3 RGB), and normal (vec3)
- `Mesh`: Container for vertex/index arrays with rendering

**Key Classes**:
- `Mesh`: Encapsulates 3D geometry data and rendering operations

**Key Features**:
- Vertex storage with position, color, and normal data
- Triangle index buffer for efficient rendering
- Static `CreateCube()` factory for unit cube (all 6 faces, different colors)
- Back-face culling state management
- Rendering via OpenGL immediate mode

**Cube Specification**:
- 24 vertices (4 per face × 6 faces) for independent face colors
- 36 indices (2 triangles per face × 6 faces)
- Unit cube bounds: [-0.5, 0.5] in all axes
- Face colors:
  - Front (Z+): Red (1, 0, 0)
  - Back (Z-): Green (0, 1, 0)
  - Right (X+): Blue (0, 0, 1)
  - Left (X-): Yellow (1, 1, 0)
  - Top (Y+): Cyan (0, 1, 1)
  - Bottom (Y-): Magenta (1, 0, 1)

**Public Methods**:
- `static Mesh CreateCube()`: Factory method creating colored cube
- `Render()`: Render mesh using OpenGL immediate mode
- `SetBackfaceCulling(bool)`: Enable/disable back-face culling
- `IsBackfaceCullingEnabled()`: Query culling state
- `GetVertexCount()`, `GetIndexCount()`: Query geometry dimensions
- `GetVertexPosition(idx)`, `GetVertexColor(idx)`, `GetVertexNormal(idx)`: Access vertex data
- `GetIndex(idx)`: Access index buffer

**Typical Usage**:
```cpp
blec::render::Mesh cube = blec::render::Mesh::CreateCube();
cube.SetBackfaceCulling(true);

// In render loop:
glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
renderer.SetModel(model);
renderer.EnableBackfaceCulling();
cube.Render();
renderer.DisableBackfaceCulling();
```

### BlockSystem Module (`include/world/block_system.h`, `src/world/block_system.cpp`)

**Purpose**: Manages voxel grid for blocks and performs view frustum culling.

**Key Structures**:
- `Block`: Represents a single block (type 0 = air, 1+ = solid)
- `AABB`: Axis-aligned bounding box for collision/intersection testing
- `FrustumPlane`: Plane equation for frustum culling
- `ViewFrustum`: Set of 6 planes defining camera view frustum
- `BlockSystem`: Main class for managing block grid and visibility

**Key Features**:
- 3D voxel grid storage with configurable dimensions
- Block get/set operations with bounds checking
- Frustum plane extraction from view-projection matrix
- AABB-frustum intersection testing for visibility culling
- Efficient block visibility counting (total blocks vs. visible blocks)
- World-space to grid-space coordinate conversion
- Test block creation for debugging

**Frustum Culling Algorithm**:
1. Extract 6 frustum planes from projection × view matrix
2. Test each non-air block's AABB against all 6 planes
3. Block is visible if AABB intersects frustum (not outside any plane)
4. Count visible blocks for performance metrics

**Public Methods**:
- `Initialize(width, height, depth, block_size)`: Set up grid dimensions
- `CreateTestBlocks()`: Create 3×3×3 test cube for demonstration
- `ExtractFrustum(view, projection)`: Extract frustum planes from matrices
- `UpdateVisibility()`: Count visible blocks using current frustum
- `GetTotalBlockCount()`: Get count of non-air blocks in world
- `GetVisibleBlockCount()`: Get count of blocks visible in frustum
- `GetBlock(x, y, z)`: Retrieve block at grid position
- `SetBlock(x, y, z, block)`: Set block at grid position
- `GetBlockAABB(x, y, z)`: Get bounding box for block

**Performance Characteristics**:
- Grid storage: O(1) access time for get/set
- Visibility update: O(N) where N = number of non-air blocks
- Frustum extraction: O(1) constant time (6 planes)
- AABB-frustum test: O(1) per block (6 plane tests max)

**Typical Usage**:
```cpp
blec::world::BlockSystem block_system;
block_system.Initialize(32, 32, 32, 1.0f);  // 32³ grid, 1 unit blocks
block_system.CreateTestBlocks();

// In main loop:
glm::mat4 view = camera.GetViewMatrix();
glm::mat4 projection = glm::perspective(...);
block_system.ExtractFrustum(view, projection);
block_system.UpdateVisibility();

uint32_t total = block_system.GetTotalBlockCount();
uint32_t visible = block_system.GetVisibleBlockCount();
```

### Debug Module (`include/debug/`, `src/debug/`)

**Purpose**: Displays real-time game state and development information.

**Key Classes**:
- `DebugOverlay`: Manages debug state and rendering

**Key Features**:
- Toggleable overlay (F12 key)
- FPS calculation and display
- Camera position and orientation display (X, Y, Z, Yaw, Pitch)
- Block system statistics (total blocks, visible blocks)
- Input state display (keys pressed, mouse position/delta)
- Error and warning tracking
- Semi-transparent background box categorized into sections

**Information Displayed**:
- **Game State**: FPS
- **Camera**: Position (X, Y, Z), Orientation (Yaw°, Pitch°)
- **Blocks**: Total non-air blocks, Visible blocks in frustum
- **Input**: Keys currently down, Last key event, Mouse position and delta
- **Issues**: Error count, Warning count (if any)

**Dependencies**:
- Input module (for state queries)
- Render module (for drawing)
- Camera module (for position/orientation)
- BlockSystem module (for block counts)

### UI Module (`include/ui/`, `src/ui/`)

**Purpose**: Manages user interface elements including crosshair, pause menu, and mouse lock state.

**Key Classes**:
- `UIManager`: Manages UI state, pause menu, and crosshair rendering
- `Button`: Simple struct for button hit detection

**Key Features**:
- **Crosshair**: Always-visible plus-sign crosshair in screen center for aiming
- **Pause Menu**: ESC-toggleable menu with Resume and Quit buttons
- **Mouse Lock**: Automatic cursor locking during gameplay (GLFW_CURSOR_DISABLED)
- **Button Interaction**: Click detection for pause menu buttons
- **Screen Adaptation**: Automatically adjusts UI positions when window resizes

**Pause Menu**:
- Semi-transparent dark background overlay (70% opacity)
- "PAUSED" title text
- Resume button (green) - resumes gameplay
- Quit button (red) - closes application
- Resume via ESC key or clicking Resume button

**Mouse Lock Behavior**:
- **Playing**: Mouse locked to window center (cursor hidden, infinite movement)
- **Paused**: Mouse unlocked (cursor visible, normal behavior)
- Uses GLFW's `GLFW_CURSOR_DISABLED` mode for seamless camera control

**Public Methods**:
- `Initialize(width, height)`: Set up UI with screen dimensions
- `TogglePause()`: Toggle between paused and playing states
- `SetPaused(bool)`: Explicitly set pause state
- `IsPaused()`: Query current pause state
- `HandleMouseClick(x, y)`: Process mouse clicks on pause menu buttons
- `RenderCrosshair(renderer)`: Draw crosshair in 2D overlay
- `RenderPauseMenu(renderer, font)`: Draw pause menu when paused
- `UpdateScreenDimensions(width, height)`: Adjust UI for window resize

**UI Layout Constants**:
- Crosshair: 20px lines with 2px thickness, 5px center gap
- Buttons: 150px × 40px with 20px vertical spacing
- Menu: Centered on screen, adapts to window size

**Typical Usage**:
```cpp
blec::ui::UIManager ui;
ui.Initialize(1280, 720);

// In main loop:
if (ui.IsPaused()) {
    // Unlock mouse, freeze gameplay
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    // Handle menu clicks
    auto action = ui.HandleMouseClick(mouse_x, mouse_y);
    if (action == UIManager::ButtonAction::Quit) { /* exit */ }
} else {
    // Lock mouse, enable gameplay
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Rendering (in 2D phase):
ui.RenderCrosshair(renderer);
ui.RenderPauseMenu(renderer, font);
```

**Dependencies**:
- Render module (for drawing primitives and text)
- Input module (for mouse position and clicks via GLFW)


## Data Flow

### Initialization Flow
```
1. main.cpp creates all module instances
2. WindowManager initializes GLFW
3. WindowManager creates window
4. WindowManager sets OpenGL context current
5. Renderer initializes (currently no-op, future shader loading)
6. InputHandler registers GLFW callbacks
7. Main loop begins
```

### Frame Update Flow
```
1. Poll GLFW events (input callbacks fire)
2. Check for ESC key → request close
3. Check for F12 key → toggle debug overlay
4. Update debug overlay (FPS calculation)
5. Get framebuffer size
6. Set viewport
7. Clear screen
8. If debug visible → render debug overlay
9. Swap buffers (display frame)
10. Reset per-frame input state (mouse delta)
```

### Shutdown Flow
```
1. Exit main loop
2. WindowManager.Shutdown()
   - Destroy window
   - Terminate GLFW
3. Return from main
```

## File Organization

```
include/
  ├── window/
  │   └── window_manager.h      # Window management interface
  ├── input/
  │   └── input_handler.h       # Input tracking interface
  ├── render/
  │   ├── renderer.h            # Rendering operations (2D and 3D)
  │   ├── camera.h              # Free-flying 3D camera
  │   ├── mesh.h                # 3D geometry and rendering
  │   └── font.h                # Bitmap font rendering
  ├── world/
  │   └── block_system.h        # Voxel grid and frustum culling
  ├── ui/
  │   └── ui_manager.h          # UI, crosshair, and pause menu
  └── debug/
      └── debug_overlay.h       # Debug overlay interface

src/
  ├── window/
  │   └── window_manager.cpp    # Window implementation
  ├── input/
  │   └── input_handler.cpp     # Input implementation
  ├── render/
  │   ├── renderer.cpp          # Rendering implementation
  │   ├── camera.cpp            # Camera implementation
  │   ├── mesh.cpp              # Mesh implementation
  │   └── font.cpp              # Font implementation + data
  ├── world/
  │   └── block_system.cpp      # Block system implementation
  ├── ui/
  │   └── ui_manager.cpp        # UI manager implementation
  ├── debug/
  │   └── debug_overlay.cpp     # Debug overlay implementation
  └── main.cpp                  # Application entry point

code_testing/
  ├── test_framework.h          # Custom testing framework
  ├── window/
  │   └── test_window_manager.cpp
  ├── input/
  │   └── test_input_handler.cpp
  ├── render/
  │   ├── test_font.cpp
  │   ├── test_renderer.cpp
  │   ├── test_camera.cpp
  │   ├── test_mesh.cpp
  │   └── test_renderer_3d.cpp
  ├── world/
  │   └── test_block_system.cpp
  ├── ui/
  │   └── test_ui_manager.cpp
  └── debug/
      └── test_debug_overlay.cpp
```

## Coding Standards

### Naming Conventions
- **Classes**: PascalCase (`WindowManager`, `InputHandler`)
- **Methods**: PascalCase (`GetFramebufferSize`, `IsKeyDown`)
- **Variables**: snake_case (`window_`, `frame_count_`)
- **Constants**: kPascalCase (`kWindowWidth`, `kFont5x7`)
- **Namespaces**: lowercase (`blec::window`, `blec::input`)

### Header Guards
- Format: `BLEC_<MODULE>_<FILE>_H`
- Example: `BLEC_WINDOW_MANAGER_H`

### Documentation
- Every public method has a comment explaining its purpose
- Complex logic has inline comments
- File headers explain module purpose

### Error Handling
- Functions return `bool` for success/failure where appropriate
- Errors logged to stderr
- Debug overlay records errors for visibility

## Testing Strategy

### Unit Tests
Each module has comprehensive unit tests covering:
- Initialization and cleanup
- Public API correctness
- Edge cases and error handling
- Integration points with other modules

### Test Framework
- Custom lightweight framework (no external dependencies)
- Automatic test registration
- Clear pass/fail reporting
- Summary statistics

### Running Tests
```bash
cmake -DBUILD_TESTS=ON ..
cmake --build .
cmake --build . --target run_tests
```

## Future Extensions

### Adding a New Module

1. **Create header** in `include/new_module/`
2. **Create implementation** in `src/new_module/`
3. **Update CMakeLists.txt** to include new source files
4. **Create tests** in `code_testing/new_module/`
5. **Update test CMakeLists.txt** to include new tests
6. **Integrate in main.cpp** as needed

### Adding New Features to Existing Modules

1. **Add method to header** with documentation
2. **Implement in source file** with comments
3. **Write tests** for new functionality
4. **Update documentation** if needed

## Performance Considerations

### Current Performance
- Target: 60 FPS (with VSync enabled)
- Minimal CPU usage (simple 2D rendering)
- Low memory footprint (<10 MB typical)

### Optimization Points
- Font rendering uses immediate mode (can be optimized with VBOs)
- Debug overlay recalculates layout every frame (can be cached)
- Input state array could be optimized (348 bools = 348 bytes)

### Future Optimizations
- Batch rendering for font characters
- Cache debug overlay layout
- Use bitset for key state (348 bits = 44 bytes)

## Security Best Practices

### Current Implementation
- No external network access
- No file I/O (except future save/load)
- No dynamic code execution
- No user data collection

### Guidelines
- Never read/write files without explicit user action
- Validate all input from GLFW callbacks
- Use fixed-size buffers for formatting (std::snprintf)
- Avoid unbounded loops or allocations

## Maintenance

### Adding Dependencies
1. Ensure cross-platform availability
2. Prefer header-only libraries
3. Use CMake FetchContent for consistency
4. Document in README and BUILDING.md

### Updating GLFW/OpenGL Versions
1. Test on all target platforms
2. Update system requirements
3. Consider backward compatibility
4. Update documentation

### Code Review Checklist
- [ ] Follows naming conventions
- [ ] Has comprehensive comments
- [ ] Includes unit tests
- [ ] No platform-specific code
- [ ] No hardcoded paths
- [ ] Error handling present
- [ ] Memory safety (RAII, smart pointers if needed)
- [ ] Const correctness

## Resources

### OpenGL Resources
- [OpenGL 2.1 Reference](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/)
- [Learn OpenGL](https://learnopengl.com/)

### GLFW Resources
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [GLFW Guide](https://www.glfw.org/docs/latest/quick.html)

### C++ Resources
- [C++ Reference](https://en.cppreference.com/)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
