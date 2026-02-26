# B-Lec

A C++ 3D voxel game prototype demonstrating modular architecture with window management, rendering, input handling, and block-based world system.

## Features

- **Modular Architecture**: Clean separation of concerns across window, input, rendering, world, and debug systems
- **3D Rendering**: OpenGL-based 3D graphics with camera control, mesh rendering, and frustum culling
- **Voxel Grid System**: Block-based world with efficient frustum culling for visibility optimization
- **Input Handling**: Keyboard and mouse input with camera movement and look controls
- **Debug Overlay**: Real-time debugging information including FPS, camera position, and block visibility counts
- **Cross-Platform**: Windows, Linux, and macOS support through GLFW and standard C++17
- **Minimal Dependencies**: Only GLFW (windowing), OpenGL (rendering), and GLM (mathematics) required
- **Lightweight Testing**: Custom testing framework with dedicated test suite for each module

## Quick Start

### Prerequisites

- CMake 3.20+
- C++17 compiler (MSVC, GCC, or Clang)
- OpenGL 2.1 or higher
- Internet access for first build (dependencies are auto-downloaded)

### Build and Run

**Windows (Visual Studio):**
```cmd
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
.\bin\Release\blec.exe
```

**Linux/macOS:**
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel
./bin/blec
```

### Controls

- **W/A/S/D**: Move forward/left/backward/right
- **Space/Ctrl**: Move up/down
- **Mouse**: Look around (mouse capture when in window)
- **F3**: Toggle debug overlay


## Architecture Overview

B-Lec follows a **modular architecture** with clear separation of concerns:

- **Window Module**: GLFW window lifecycle and OS integration
- **Input Module**: Keyboard and mouse state tracking
- **Render Module**: OpenGL operations, font rendering, camera, and mesh management
- **World Module**: Voxel grid system with frustum culling optimization
- **Debug Module**: Real-time debugging overlay and statistics

For detailed architecture, design principles, and module interactions, see [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

## Dependencies

Dependencies are automatically downloaded during build configuration:

- **GLFW 3.4**: Window creation and input event management
- **GLM 1.0.2**: Header-only mathematics library for 3D transformations
- **OpenGL**: Native graphics API (already on most systems)

## Building with Tests

To build and run the test suite:

**Windows:**
```cmd
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON ..
cmake --build . --config Release
cmake --build . --target run_tests
```

**Linux/macOS:**
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON ..
cmake --build . --parallel
cmake --build . --target run_tests
```

## Contributing

We welcome contributions! This section explains how to participate in development.

### Setting Up Your Development Environment

**1. Clone and setup:**
```bash
git clone https://github.com/yourusername/B-Lec.git
cd B-Lec
mkdir build
cd build
```

**2. Configure with tests enabled:**
```bash
# Windows
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON ..

# Linux/macOS
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON ..
```

**3. Build and verify:**
```bash
cmake --build .
cmake --build . --target run_tests
```

### Development Workflow

**1. Understanding the Code**
   - Read the module header files in `include/` for API documentation
   - Check test files in `code_testing/` for usage examples
   - See [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md) for detailed module guides

**2. Code Style**
   - Classes and functions: `PascalCase`
   - Constants: `kPascalCase` (e.g., `kWindowWidth`)
   - Variables: `snake_case`
   - Member variables: `snake_case_` (with trailing underscore)
   - Use header guards: `#ifndef BLEC_MODULE_CLASS_H`
   - Namespace: `blec::module::ClassName`

**3. Making Changes**

   **Adding a new function:**
   ```cpp
   // 1. Add declaration to header
   /// Explain what this function does
   bool MyNewFunction();
   
   // 2. Implement in source file
   bool ClassName::MyNewFunction() {
       // Implementation with clear comments
       return true;
   }
   
   // 3. Write test case
   TEST_CASE(TestMyNewFunction) {
       ASSERT_TRUE(MyNewFunction());
   }
   ```

   **Modifying existing functions:**
   - Update header declaration if signature changes
   - Update implementation in source file
   - Find all usages with: `grep -r "FunctionName" src/ include/`
   - Update affected tests
   - Update documentation comments

   **Adding a new module:**
   - Create header: `include/module/class.h`
   - Create implementation: `src/module/class.cpp`
   - Add source file to `CMakeLists.txt`
   - Create test file: `code_testing/module/test_class.cpp`
   - Update test CMakeLists.txt
   - Integrate in `main.cpp` if needed

**4. Testing**

   Always run tests before committing:
   ```bash
   cmake --build . --target run_tests
   ```

   Follow these guidelines:
   - One test case per function/behavior
   - Test name starts with "Test"
   - Test both success and error cases
   - Use clear assertion messages

**5. Documentation**

   When making changes, update:
   - Code comments explaining "why", not just "what"
   - [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for structural changes
   - [docs/BUILDING.md](docs/BUILDING.md) if build process changes
   - This README if adding user-facing features

**6. Committing and Pull Requests**

   Before submitting a PR, ensure:
   - Code compiles without warnings
   - All tests pass
   - New code has unit tests
   - Code follows naming conventions
   - Complex logic is documented
   - Commit messages are clear and descriptive

### Resources for Contributors

- [Building Guide](docs/BUILDING.md) - Comprehensive build instructions
- [Architecture Documentation](docs/ARCHITECTURE.md) - System design and module details
- [Development Guide](docs/DEVELOPMENT.md) - In-depth developer reference with examples
- Header file comments - API documentation and usage
- Test files - Code examples and edge cases

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) file for details.

## Getting Help

- Check the [Development Guide](docs/DEVELOPMENT.md) for detailed explanations and code examples
- Review test files for usage patterns
- Search existing code for similar implementations
- Open an issue to discuss features or bugs

