# B-Lec Development Guide

Welcome to the B-Lec project! This guide will help you understand the codebase and contribute effectively.

## Quick Start for Developers

### 1. Setting Up Your Environment

**Clone the repository**:
```bash
git clone https://github.com/yourusername/B-Lec.git
cd B-Lec
```

**Create build directory**:
```bash
mkdir build
cd build
```

**Configure with tests enabled** (recommended for development):
```bash
# Windows
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON ..

# Linux/macOS
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON ..
```

**Build**:
```bash
cmake --build .
```

**Run tests**:
```bash
cmake --build . --target run_tests
```

### 2. Project Structure Quick Guide

- **include/**: Public API headers, organized by module
- **src/**: Implementation files, same organization as include/
- **code_testing/**: Unit tests, one per module
- **CMakeLists.txt**: Build configuration
- **ARCHITECTURE.md**: Detailed architecture documentation
- **BUILDING.md**: Build instructions for users

## Understanding the Code

### Module Overview

**Window Module** (`include/window/window_manager.h`)
- Manages GLFW window lifecycle
- Handles window events and properties
- ~100 lines of code per file

**Input Module** (`include/input/input_handler.h`)
- Tracks keyboard and mouse state
- Provides input querying APIs
- ~150 lines of code per file

**Render Module** (`include/render/`)
- `renderer.h`: Basic OpenGL operations
- `font.h`: Bitmap font rendering
- ~200 lines of code total

**Debug Module** (`include/debug/debug_overlay.h`)
- Displays real-time debug information
- Manages debug state and rendering
- ~150 lines of code per file

### Code Style

**Header files** (include your guards at the top):
```cpp
#ifndef BLEC_MODULE_CLASS_H
#define BLEC_MODULE_CLASS_H

namespace blec {
namespace module {

/// Brief description of what this class does
class ClassName {
public:
    /// Explain what this does
    bool PublicMethod();

private:
    // Private members start with underscore
    int member_;
};

} // namespace module
} // namespace blec

#endif // BLEC_MODULE_CLASS_H
```

**Implementation files**:
```cpp
#include "module/class.h"
#include <necessary_headers>

namespace blec {
namespace module {

// Implementation with clear comments explaining logic
bool ClassName::PublicMethod() {
    // Do something
    return true;
}

} // namespace module
} // namespace blec
```

### Naming Examples

✅ **Good**:
```cpp
class WindowManager { };
bool InitializeGLFW();
void GetFramebufferSize(int* width, int* height);
const int kWindowWidth = 1280;
std::string window_title_;
```

❌ **Bad**:
```cpp
class WindowMgr { };  // Don't abbreviate
bool init_glfw();  // Methods should be PascalCase
void getFramebufferSize(int* w, int* h);  // Avoid abbreviations
#define WINDOW_WIDTH 1280  // Use constexpr or const
std::string windowTitle;  // Member variables should be snake_case with _
```

## Working with Modules

### Creating a New Function

1. **Add declaration to header**:
```cpp
/// Returns the current frame rate in frames per second
double GetCurrentFPS() const;
```

2. **Implement in source file**:
```cpp
double WindowManager::GetCurrentFPS() const {
    // Implementation with logic comments if complex
    return fps_;
}
```

3. **Write tests**:
```cpp
TEST_CASE(TestGetCurrentFPS) {
    WindowManager wm;
    ASSERT_GE(wm.GetCurrentFPS(), 0.0);
    ASSERT_LE(wm.GetCurrentFPS(), 1000.0);  // Reasonable upper bound
}
```

### Modifying Existing Functions

1. **Check all usages** with grep/search:
```bash
grep -r "FunctionName" src/ include/
```

2. **Update signature if needed** in header
3. **Update implementation** in source
4. **Update tests** to verify behavior
5. **Update documentation** in comments

### Adding a New Module

**Example: Adding a "World" module for game world management**

1. **Create header** `include/world/world.h`:
```cpp
#ifndef BLEC_WORLD_WORLD_H
#define BLEC_WORLD_WORLD_H

namespace blec {
namespace world {

class World {
public:
    World();
    ~World();
    
    bool Initialize();
    void Update(double deltaTime);
    void Shutdown();
    
private:
    // Members...
};

} // namespace world
} // namespace blec

#endif // BLEC_WORLD_WORLD_H
```

2. **Create implementation** `src/world/world.cpp`

3. **Update CMakeLists.txt**:
```cmake
add_executable(blec
    # ... existing sources ...
    src/world/world.cpp
)
```

4. **Create tests** `code_testing/world/test_world.cpp`

5. **Update test CMakeLists.txt**:
```cmake
set(TEST_SOURCES
    # ... existing tests ...
    world/test_world.cpp
)
```

6. **Integrate in main.cpp** if needed

## Working with 3D Rendering

### Understanding the 3D Pipeline

The render module now supports 3D graphics with camera, mesh, and matrix operations:

1. **Camera** (`camera.h`): Manages viewpoint and controls
2. **Mesh** (`mesh.h`): Stores geometry and renders 3D objects
3. **Renderer** 3D methods: Perspective projection and state management

### Using the Camera Class

**Basic setup**:
```cpp
#include "render/camera.h"

blec::render::Camera camera;

// Initialize with position and target
camera.Initialize(glm::vec3(0.0f, 0.0f, 5.0f),  // Camera position
                  glm::vec3(0.0f, 0.0f, 0.0f));  // Look-at target

// Configure speeds (optional, have sensible defaults)
camera.SetMovementSpeed(5.0f);      // Units per second
camera.SetRotationSpeed(0.005f);    // Radians per pixel
```

**Handle input in main loop**:
```cpp
// WASD movement
if (input_handler.IsKeyDown(GLFW_KEY_W)) {
    camera.MoveForward(delta_time * 5.0);
}
if (input_handler.IsKeyDown(GLFW_KEY_S)) {
    camera.MoveForward(-delta_time * 5.0);
}
if (input_handler.IsKeyDown(GLFW_KEY_A)) {
    camera.MoveRight(-delta_time * 5.0);
}
if (input_handler.IsKeyDown(GLFW_KEY_D)) {
    camera.MoveRight(delta_time * 5.0);
}

// Space/Ctrl for up/down
if (input_handler.IsKeyDown(GLFW_KEY_SPACE)) {
    camera.MoveUp(delta_time * 5.0);
}
if (input_handler.IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
    camera.MoveUp(-delta_time * 5.0);
}

// Mouse look
double mouse_dx = 0.0, mouse_dy = 0.0;
input_handler.GetMouseLookDelta(&mouse_dx, &mouse_dy);
camera.Yaw(mouse_dx * 0.005f);
camera.Pitch(-mouse_dy * 0.005f);  // Inverted Y for natural look

// Update camera state
camera.Update(delta_time);
```

**Get camera matrices for rendering**:
```cpp
glm::mat4 view = camera.GetViewMatrix();
renderer.SetView(view);
```

### Using the Mesh Class

**Create a colored cube**:
```cpp
#include "render/mesh.h"

// Create a cube with 6 different colored faces
blec::render::Mesh cube = blec::render::Mesh::CreateCube();

// Enable back-face culling for optimization
cube.SetBackfaceCulling(true);
```

**Render a mesh**:
```cpp
// In render loop:
renderer.Begin3D(window_width, window_height, 45.0f);  // 45° FOV

// Set up matrices
glm::mat4 view = camera.GetViewMatrix();
renderer.SetView(view);

glm::mat4 model = glm::mat4(1.0f);  // Identity = object at origin
renderer.SetModel(model);

// Enable features
renderer.EnableDepthTest();
renderer.EnableBackfaceCulling();

// Render the geometry
cube.Render();

renderer.DisableBackfaceCulling();
renderer.End3D();
```

### Complete 3D Rendering Loop Example

```cpp
#include "render/camera.h"
#include "render/mesh.h"

// Setup phase
blec::render::Camera camera;
camera.Initialize(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

blec::render::Mesh cube = blec::render::Mesh::CreateCube();
cube.SetBackfaceCulling(true);

// Main loop
while (!window_manager.ShouldClose()) {
    // ... input handling (see above) ...
    
    // Update camera
    camera.Update(delta_time);
    
    // Render 3D scene
    renderer.Begin3D(fb_width, fb_height, 45.0f);
    
    renderer.SetView(camera.GetViewMatrix());
    
    // Render cube
    glm::mat4 cube_model = glm::mat4(1.0f);
    renderer.SetModel(cube_model);
    renderer.EnableDepthTest();
    renderer.EnableBackfaceCulling();
    cube.Render();
    
    renderer.End3D();
    
    // Render 2D overlay on top
    renderer.Begin2D(fb_width, fb_height);
    // ... render debug overlay, HUD, etc ...
    renderer.End2D();
    
    window_manager.SwapBuffers();
    input_handler.ResetMouseDelta();
}
```

### 3D Rendering Tips

**Coordinate System**:
- X-axis: Points to the right
- Y-axis: Points up
- Z-axis: Points towards the camera (right-hand rule)

**Camera Movement**:
- Use `MoveForward()` for depth (along view direction)
- Use `MoveRight()` for lateral movement (strafe)
- Use `MoveUp()` for vertical movement
- Use `Yaw()` to rotate left/right
- Use `Pitch()` to look up/down

**Mesh Colors**:
The default cube has 6 faces with different colors:
- Front (Z+): Red
- Back (Z-): Green
- Right (X+): Blue
- Left (X-): Yellow
- Top (Y+): Cyan
- Bottom (Y-): Magenta

**Performance**:
- Always enable `EnableBackfaceCulling()` to skip rendering hidden faces
- Enable `EnableDepthTest()` so objects render in correct order
- Disable culling/depth before rendering 2D overlay

**Common Issues**:
- **White/blank screen**: Check camera position isn't inside the cube
- **Inverted controls**: Change sign of mouse delta in Pitch
- **Flickering**: Check depth test is enabled before rendering 3D
- **Objects disappear**: Check near/far planes in Begin3D FOV range objects

## Working with BlockSystem

### Understanding the Voxel Grid

The `BlockSystem` manages a 3D voxel grid where each voxel (block) can be air (empty) or solid. It includes frustum culling to efficiently determine which blocks are visible from the camera's perspective.

**Key concepts**:
- **Block**: A single voxel with a type (0 = air, 1+ = solid types)
- **Grid**: 3D array of blocks with configurable dimensions
- **AABB**: Axis-aligned bounding box for each block
- **Frustum**: 6-plane view frustum extracted from camera matrices
- **Visibility**: Blocks that intersect the view frustum

### Basic Setup

**Initialize the block system**:
```cpp
#include "world/block_system.h"

blec::world::BlockSystem block_system;

// Initialize with grid dimensions and block size
block_system.Initialize(32,    // Width (X)
                        32,    // Height (Y)
                        32,    // Depth (Z)
                        1.0f); // Block size in world units
```

**Create some test blocks**:
```cpp
// Creates a 3×3×3 cube of solid blocks at the grid center
block_system.CreateTestBlocks();

// Get block counts
int total = block_system.GetTotalBlockCount();      // Non-air blocks
int visible = block_system.GetVisibleBlockCount();  // Blocks in frustum
```

### Working with Individual Blocks

**Get and set blocks**:
```cpp
// Set a block (returns true if successful)
bool success = block_system.SetBlock(10, 5, 10, 1);  // Set solid block at (10,5,10)

// Get block type
uint8_t block_type = block_system.GetBlock(10, 5, 10);
if (block_type == 0) {
    // It's air
} else {
    // It's a solid block
}

// Check if position is valid
if (x >= 0 && x < 32 && y >= 0 && y < 32 && z >= 0 && z < 32) {
    // Position is within bounds
}
```

**Convert between grid and world coordinates**:
```cpp
// Get world-space position of a block's center
glm::vec3 world_pos = block_system.GetBlockWorldPosition(10, 5, 10);

// Get bounding box of a block
blec::world::AABB bbox = block_system.GetBlockAABB(10, 5, 10);
// bbox.min and bbox.max are glm::vec3
```

### Frustum Culling

**Update visibility each frame**:
```cpp
// In your main loop:

// 1. Create projection matrix
int fb_width, fb_height;
window_manager.GetFramebufferSize(&fb_width, &fb_height);
float aspect = static_cast<float>(fb_width) / fb_height;
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

// 2. Get view matrix from camera
glm::mat4 view = camera.GetViewMatrix();

// 3. Extract frustum from matrices
block_system.ExtractFrustum(view, projection);

// 4. Update which blocks are visible
block_system.UpdateVisibility();

// 5. Query results
int total_blocks = block_system.GetTotalBlockCount();
int visible_blocks = block_system.GetVisibleBlockCount();
```

**Pass data to debug overlay**:
```cpp
debug_overlay.SetBlockCounts(total_blocks, visible_blocks);
```

### Complete Example

```cpp
#include "world/block_system.h"
#include "render/camera.h"

// Setup phase
blec::world::BlockSystem block_system;
block_system.Initialize(32, 32, 32, 1.0f);
block_system.CreateTestBlocks();

blec::render::Camera camera;
camera.Initialize(glm::vec3(0.0f, 15.0f, 30.0f), glm::vec3(16.0f, 16.0f, 16.0f));

// Main loop
while (!window_manager.ShouldClose()) {
    // ... handle input and update camera ...
    
    // Update frustum and visibility
    int fb_width, fb_height;
    window_manager.GetFramebufferSize(&fb_width, &fb_height);
    float aspect = static_cast<float>(fb_width) / fb_height;
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    
    block_system.ExtractFrustum(view, projection);
    block_system.UpdateVisibility();
    
    // Update debug info
    glm::vec3 cam_pos = camera.GetPosition();
    debug_overlay.SetCameraPosition(cam_pos.x, cam_pos.y, cam_pos.z);
    debug_overlay.SetCameraOrientation(camera.GetYaw(), camera.GetPitch());
    debug_overlay.SetBlockCounts(
        block_system.GetTotalBlockCount(),
        block_system.GetVisibleBlockCount()
    );
    
    // ... rendering ...
}
```

### Performance Characteristics

**Time Complexity**:
- `GetBlock()/SetBlock()`: O(1) - Direct array access
- `ExtractFrustum()`: O(1) - Matrix operations and plane extraction
- `UpdateVisibility()`: O(N) where N = total non-air blocks
- `GetBlockAABB()`: O(1) - Simple coordinate math

**Memory Usage**:
- Grid storage: width × height × depth bytes (1 byte per block)
- Example: 32×32×32 grid = 32,768 bytes (~32 KB)

**Optimization Tips**:
- Only call `UpdateVisibility()` when camera moves or blocks change
- Grid dimensions should be powers of 2 for cache efficiency
- Test blocks are positioned at grid center, ensuring they're visible from default camera position

### Common Patterns

**Building structures**:
```cpp
// Create a floor
for (int x = 0; x < 32; x++) {
    for (int z = 0; z < 32; z++) {
        block_system.SetBlock(x, 0, z, 1);  // Solid block at y=0
    }
}

// Create a wall
for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 32; x++) {
        block_system.SetBlock(x, y, 0, 1);  // Wall at z=0
    }
}
```

**Checking visibility before rendering**:
```cpp
for (int z = 0; z < 32; z++) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            uint8_t block = block_system.GetBlock(x, y, z);
            if (block == 0) continue;  // Skip air
            
            // Get world position and AABB
            glm::vec3 pos = block_system.GetBlockWorldPosition(x, y, z);
            blec::world::AABB bbox = block_system.GetBlockAABB(x, y, z);
            
            // Only render if visible (already filtered by UpdateVisibility)
            // Your rendering code here...
        }
    }
}
```

### Troubleshooting

**No blocks visible**:
- Check camera position isn't too far from blocks
- Verify frustum near/far planes encompass block positions
- Ensure `UpdateVisibility()` is called after `ExtractFrustum()`
- Check projection matrix field of view isn't too narrow

**All blocks always visible**:
- Verify frustum is being extracted with correct matrices
- Check that view and projection matrices are properly computed
- Ensure projection matrix matches the one used for rendering

**Incorrect block counts**:
- Use `CreateTestBlocks()` for known 27-block configuration
- Verify grid is initialized before setting blocks
- Check bounds when setting blocks (out-of-bounds calls return false)

## Testing Guidelines


### Writing Good Tests

**❌ Bad test**:
```cpp
TEST_CASE(TestEverything) {
    MyClass obj;
    obj.DoSomething();  // Unclear what we're testing
}
```

**✅ Good test**:
```cpp
TEST_CASE(TestInitialization) {
    MyClass obj;
    ASSERT_TRUE(obj.Initialize());
    ASSERT_TRUE(obj.IsInitialized());
}

TEST_CASE(TestInvalidInput) {
    MyClass obj;
    ASSERT_FALSE(obj.SetValue(-1));  // Negative values invalid
}

TEST_CASE(TestStateTransition) {
    MyClass obj;
    obj.Start();
    ASSERT_TRUE(obj.IsRunning());
    
    obj.Stop();
    ASSERT_FALSE(obj.IsRunning());
}
```

### Test Organization

- One test class/function per test case
- Test name starts with "Test"
- Test structure: Setup → Execute → Verify
- Clear assertion messages
- Test both success and error cases

### Running Tests During Development

Test locally before committing:
```bash
cmake --build . --target run_tests
```

Fix any failing tests immediately - they help catch regressions.

## Common Tasks

### Debugging a Crash

1. **Build with debug symbols**:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

2. **Run in debugger**:
```bash
# GDB (Linux/macOS)
gdb ./bin/blec

# Visual Studio (Windows)
# Open .sln and press F5
```

3. **Add debug logging**:
```cpp
std::fprintf(stderr, "DEBUG: variable value = %d\n", value);
```

### Adding a New Keyboard Shortcut

1. **Update InputHandler** (if needed) to recognize the key
2. **Add handler in main.cpp**:
```cpp
if (input_handler.IsKeyDown(GLFW_KEY_F3)) {
    // Your code here
}
```
3. **Document in controls section** of README

### Improving Performance

1. **Profile the application**:
   - Use platform profilers (perf on Linux, Instruments on macOS, VTune on Windows)
   - Check FPS in debug overlay

2. **Identify bottleneck**:
   - Is it CPU? GPU? Rendering?

3. **Optimize** the bottleneck:
   - Don't premature optimize
   - Measure before and after
   - Consider trade-offs (readability vs performance)

4. **Verify with tests** that optimization doesn't break functionality

## Documentation

### Commenting Code

**Always explain "why", not just "what"**:

```cpp
// ❌ Not helpful - the code is obvious
int width = 800;  // Set width to 800

// ✅ Helpful - explains the reason
// Use 1280 width to match primary monitor resolution on modern laptops
constexpr int kWindowWidth = 1280;
```

### Updating Architecture Documentation

If you make structural changes:
1. Update ARCHITECTURE.md with new design
2. Update diagrams if needed
3. Update module descriptions
4. Update data flow if changed

### Updating User Guides

If you add user-facing features:
1. Update README.md with new features
2. Update BUILDING.md if build process changed
3. Update controls in BUILDING.md if input changed

## Pull Request Checklist

Before submitting a pull request:

- [ ] Code compiles without warnings
- [ ] All tests pass
- [ ] New code has unit tests
- [ ] Code follows naming conventions
- [ ] Comments explain complex logic
- [ ] No hardcoded paths or magic numbers
- [ ] Documentation updated
- [ ] Commit messages are clear

## Helpful Commands

**Search for a function**:
```bash
grep -r "FunctionName" .
```

**Find all TODOs**:
```bash
grep -r "TODO" src/ include/
```

**Count lines of code**:
```bash
find src include -name "*.cpp" -o -name "*.h" | xargs wc -l
```

**Format code** (if using clang-format):
```bash
find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

**Check for compilation warnings**:
```bash
# Linux/macOS
cmake -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic -Werror" ..

# Then build and fix warnings
```

## Getting Help

### Understanding a Module
1. Read the header file comments
2. Check the ARCHITECTURE.md section
3. Look at the test file for usage examples
4. Search for usages in main.cpp or other modules

### Debugging an Issue
1. Check if there's a failing test
2. Add debug output to identify the problem
3. Write a test that reproduces the issue
4. Fix the issue
5. Verify test now passes

### Contributing a Feature
1. Create an issue describing the feature
2. Discuss design with maintainers
3. Create a branch for the feature
4. Follow the guidelines above
5. Submit PR with reference to the issue

## Code Review

### Being Reviewed

- Respond to feedback constructively
- Ask questions if feedback is unclear
- Mark conversations as resolved when done
- Thank reviewer for their time

### Reviewing Others' Code

- Look for clarity and correctness
- Suggest improvements kindly
- Ask questions to understand intent
- Approve when satisfied

## Resource List

- **BUILDING.md**: How to build the project
- **ARCHITECTURE.md**: System design and module details
- **code_testing/README.md**: Testing framework and examples
- **Include file comments**: Method documentation
- **Test files**: Usage examples and edge cases

Good luck! Happy coding! 🚀
