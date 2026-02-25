# B-Lec Development Guide

This guide explains how to build, develop, and contribute to the B-Lec electricity simulator.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Building the Project](#building-the-project)
3. [Project Structure](#project-structure)
4. [Code Style Guidelines](#code-style-guidelines)
5. [Adding New Features](#adding-new-features)
6. [Testing](#testing)

## Prerequisites

### Required Software

- **CMake** 3.20 or higher
- **C++ Compiler** with C++17 support:
  - MSVC 2019+ (Visual Studio 2019+) for Windows
  - GCC 7+ for Linux
  - Clang 5+ for macOS
- **Git** for version control

### System Dependencies

#### Windows
```powershell
# Using vcpkg (recommended)
vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libglfw3-dev \
    libglm-dev \
    pkg-config
```

#### macOS
```bash
brew install cmake glfw3 glm
```

## Building the Project

### Windows (Visual Studio)

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -G "Visual Studio 16 2019" -A x64 ..

# Build
cmake --build . --config Release

# Run
.\bin\Release\blec.exe
```

### Linux/macOS

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build (using as many cores as available)
cmake --build . --parallel $(nproc)

# Run
./bin/blec
```

### Debug Build

For debugging with symbols:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Project Structure

```
B-Lec/
├── CMakeLists.txt              # Main build configuration
├── README.md                   # Project overview
├── LICENSE                     # Open source license
├── docs/
│   ├── ARCHITECTURE.md         # System architecture
│   ├── DEVELOPMENT.md          # This file
│   └── API.md                  # API documentation (future)
├── ElectricitySimulator/
│   ├── include/
│   │   ├── core/              # Application and system interfaces
│   │   ├── rendering/         # Rendering system headers
│   │   ├── world/             # World and block system headers
│   │   ├── camera/            # Camera system headers
│   │   ├── input/             # Input management headers
│   │   ├── ui/                # UI system headers
│   │   └── persistence/       # Serialization headers
│   ├── src/                   # Implementation files
│   │   ├── (mirrors include structure)
│   ├── assets/                # Game assets (textures, models, etc.)
│   ├── external/              # External libraries (GLFW, GLM, GLAD)
│   └── CMakeLists.txt         # Sub-project build configuration
└── build/                     # Output directory (generated)
    └── bin/
        └── blec               # Compiled executable
```

## Code Style Guidelines

### Naming Conventions

- **Classes**: `PascalCase` (e.g., `Renderer`, `InputManager`)
- **Functions**: `PascalCase` (e.g., `Initialize()`, `Render()`)
- **Variables**: `snake_case` (e.g., `window_width`, `is_running`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `CHUNK_SIZE`, `MAX_CHUNKS`)
- **Member variables**: `snake_case_` (with trailing underscore, e.g., `window_width_`)
- **Enum values**: `UPPER_SNAKE_CASE` (e.g., `COPPER_WIRE`, `AIR`)

### File Organization

Each system should have:
- **Header file** in `include/{system}/` with full documentation
- **Implementation** in `src/{system}/` with the same filename
- **Minimal includes** in headers (use forward declarations when possible)

### Documentation

All public classes and functions must have:
- **Doxygen comments** with `@brief` and `@param` tags
- **Clear descriptions** of parameters and return values
- **Examples** for complex functionality

**Example:**
```cpp
/**
 * @brief Initialize the renderer with window dimensions
 * @param window_width Width of the rendering window in pixels
 * @param window_height Height of the rendering window in pixels
 * @return true if initialization was successful, false otherwise
 */
bool Initialize(uint32_t window_width, uint32_t window_height);
```

### Code Quality

- **Use `const`** for immutable references and read-only methods
- **Smart pointers** for memory management (`std::unique_ptr`, `std::shared_ptr`)
- **avoid raw pointers** except for temporary references and GLFW callbacks
- **Error handling**: Log errors to `std::cerr` and return meaningful error codes
- **Include guards**: Use `#pragma once` for headers

### Formatting

- **Indentation**: 4 spaces (no tabs)
- **Line length**: Maximum 100 characters when practical
- **Braces**: Opening brace on same line (K&R style with C++11 modifications)

```cpp
if (condition) {
    statement;
} else {
    statement;
}
```

## Adding New Features

### Adding a New Block Type

1. Add to `BlockType` enum in [include/world/block.h](../ElectricitySimulator/include/world/block.h):
```cpp
enum class BlockType : uint8_t {
    // ... existing types
    MY_BLOCK = 9,
    BLOCK_TYPE_COUNT
};
```

2. Add info to `block_infos_` array in [src/world/block.cpp](../ElectricitySimulator/src/world/block.cpp):
```cpp
const BlockInfo BlockRegistry::block_infos_[] = {
    // ... existing blocks
    {"My Block", "Description", true, glm::vec3(0.5f, 0.5f, 0.5f)},
};
```

### Adding a New System

1. Create header in `include/{system}/{system}.h`
2. Create implementation in `src/{system}/{system}.cpp`
3. Add class member and initialization in `Application` class
4. Update `CMakeLists.txt` to include new source files
5. Document in [ARCHITECTURE.md](./ARCHITECTURE.md)

### Common Modifications

**Changing chunk size:**
- Modify `CHUNK_SIZE` constant in [include/world/chunk.h](../ElectricitySimulator/include/world/chunk.h)
- Regenerate affected chunk data

**Adjusting camera speed:**
- Modify `move_speed` in `Application::ProcessInput()` in [src/application.cpp](../ElectricitySimulator/src/application.cpp)

**Changing viewport size:**
- Modify `window_width_` and `window_height_` in [src/application.cpp](../ElectricitySimulator/src/application.cpp)

## Testing

### Build Verification

After building, verify the executable runs:

```bash
./bin/blec
```

Expected output:
```
Starting B-Lec Electricity Simulator...
Application initialized successfully
```

### Manual Testing Checklist

- [ ] Window opens and displays rendered blocks
- [ ] Can move camera with WASD keys
- [ ] Can move up/down with Space/Shift
- [ ] ESC key closes the application
- [ ] No console errors on startup
- [ ] Frame rate is stable (60+ FPS)

### Performance Profiling

For profiling on different platforms:

**Windows (Visual Studio):**
- Use built-in profiler: Debug → Performance Profiler

**Linux/macOS:**
```bash
# Build with debug symbols
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build .

# Profile with perf (Linux)
perf record ./bin/blec
perf report
```

## Common Issues

### CMake Not Finding Libraries

**Solution**: Set the library path explicitly:
```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/libraries ..
```

### Compilation Errors

**Missing GLM headers:**
- Ensure GLM is installed and CMake can find it
- Verify `find_package(glm)` in CMakeLists.txt

**GLFW window error:**
- Verify GLFW is properly installed
- Check that your system supports OpenGL 4.6

### Runtime Issues

**Black screen on startup:**
- Check that chunk generation is working
- Verify camera position is valid

## Getting Help

1. Check [ARCHITECTURE.md](./ARCHITECTURE.md) for system design
2. Review existing code for patterns and conventions
3. Check error output in console
4. Create an issue on GitHub with detailed description

## Contributing

When making changes:

1. Create a feature branch: `git checkout -b feature/my-feature`
2. Follow code style guidelines
3. Add documentation for new functionality
4. Test thoroughly before committing
5. Write clear commit messages
6. Submit a pull request with description of changes
