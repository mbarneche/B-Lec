# Building the prototype

This is a C++ 3D voxel game prototype with modular architecture: window management, input handling, 2D/3D rendering, camera control, block system with frustum culling, and debug overlay.

## Project Structure

```
B-Lec/
├── include/              # Header files
│   ├── window/          # Window management
│   ├── input/           # Input handling
│   ├── render/          # Rendering, fonts, camera, and mesh
│   ├── world/           # Block system and frustum culling
│   └── debug/           # Debug overlay
├── src/                 # Implementation files
│   ├── window/
│   ├── input/
│   ├── render/          # Includes camera.cpp and mesh.cpp
│   ├── world/           # Includes block_system.cpp
│   ├── debug/
│   └── main.cpp         # Entry point
├── code_testing/        # Unit tests
│   ├── test_framework.h
│   ├── window/
│   ├── input/
│   ├── render/
│   ├── world/
│   └── debug/
└── CMakeLists.txt       # Build configuration
```

## Prerequisites

- CMake 3.20+
- C++17 compiler (MSVC, GCC, or Clang)
- Internet access for first configure (GLFW and GLM are fetched by CMake)
- OpenGL 2.1 or higher (for 3D rendering)

## Dependencies

The project uses FetchContent to automatically download dependencies:

- **GLFW 3.4**: Window creation and input management
- **GLM 0.9.9.8**: Mathematics library for 3D transformations (header-only)

Both are downloaded automatically during the first CMake configure.

## Configure and build

### Windows (Visual Studio)

```cmd
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

Run:

```cmd
.\bin\Release\blec.exe
```

### Linux/macOS

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel
```

Run:

```bash
./bin/blec
```

## Building with Tests

Add `-DBUILD_TESTS=ON` to the cmake configuration command:

### Windows:
```cmd
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON ..
cmake --build . --config Release
```

### Linux/macOS:
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON ..
cmake --build . --parallel
```

Run tests:
```bash
cmake --build build --target run_tests
```

See [code_testing/README.md](code_testing/README.md) for detailed testing information.

## Controls

### General
- F12: Toggle debug overlay
- ESC: Toggle pause menu (unlocks mouse, shows Resume/Quit buttons)

### 3D Controls (Camera)
- W: Move forward
- S: Move backward
- A: Strafe left
- D: Strafe right
- SPACE: Move up
- CTRL: Move down
- MOUSE: Look around (move mouse to rotate camera view)

## Module Overview

### Window Module
Manages GLFW window creation, lifecycle, and properties. Cross-platform window management with OpenGL context.

### Input Module
Handles keyboard and mouse input tracking, with callbacks and state queries. Includes mouse look delta for camera control.

### Render Module
Provides 2D and 3D rendering operations:
- `Renderer`: 2D drawing primitives, 3D matrix setup, and OpenGL state management
- `BitmapFont`: 5x7 pixel font rendering for text display
- `Camera`: Free-flying 3D camera with WASD movement and mouse look (yaw/pitch rotation)
- `Mesh`: 3D geometry storage and rendering with back-face culling

### World Module
Manages the voxel game world and optimization:
- `BlockSystem`: 3D voxel grid with block storage and queries
- **Frustum Culling**: Automatically determines which blocks are visible in the camera's view to optimize rendering
- Configurable grid dimensions and block sizes

### UI Module
Manages user interface and game state:
- `UIManager`: Crosshair, pause menu, and mouse lock management
- **Crosshair**: Always-visible crosshair in screen center for aiming
- **Pause Menu**: ESC-toggleable menu with Resume and Quit buttons
- **Mouse Lock**: Automatic cursor locking during gameplay for seamless camera control

### Debug Module
Displays comprehensive real-time debug information including:
- FPS and performance metrics
- Camera position and orientation
- Block counts (total and visible in frustum)
- Input state and error tracking
