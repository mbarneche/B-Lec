# Building the prototype

This is a C++ game prototype with modular architecture: window management, input handling, rendering, and debug overlay.

## Project Structure

```
B-Lec/
├── include/              # Header files
│   ├── window/          # Window management
│   ├── input/           # Input handling
│   ├── render/          # Rendering and fonts
│   └── debug/           # Debug overlay
├── src/                 # Implementation files
│   ├── window/
│   ├── input/
│   ├── render/
│   ├── debug/
│   └── main.cpp         # Entry point
├── code_testing/        # Unit tests
│   ├── test_framework.h
│   ├── window/
│   ├── input/
│   ├── render/
│   └── debug/
└── CMakeLists.txt       # Build configuration
```

## Prerequisites

- CMake 3.20+
- C++17 compiler (MSVC, GCC, or Clang)
- Internet access for first configure (GLFW is fetched by CMake)

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

- F12: Toggle debug overlay
- ESC: Exit

## Module Overview

### Window Module
Manages GLFW window creation, lifecycle, and properties. Cross-platform window management.

### Input Module
Handles keyboard and mouse input tracking, with callbacks and state queries.

### Render Module
Provides basic OpenGL rendering operations, bitmap font rendering, and 2D drawing primitives.

### Debug Module
Displays real-time debug information including FPS, input state, and error tracking.
