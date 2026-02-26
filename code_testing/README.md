# B-Lec Unit Testing

This directory contains unit tests for all B-Lec modules using a custom lightweight testing framework.

## Test Framework

The `test_framework.h` is a simple, dependency-free testing framework that provides:
- Test case definitions with `TEST_CASE(name)`
- Assertion macros: `ASSERT_TRUE`, `ASSERT_FALSE`, `ASSERT_EQ`, `ASSERT_NE`, etc.
- Automatic test registration and execution
- Test result summary

## Test Structure

```
code_testing/
├── test_framework.h           # Custom testing framework
├── CMakeLists.txt             # Test build configuration
├── window/
│   └── test_window_manager.cpp
├── input/
│   └── test_input_handler.cpp
├── render/
│   ├── test_font.cpp
│   └── test_renderer.cpp
└── debug/
    └── test_debug_overlay.cpp
```

## Building Tests

### Configure with tests enabled:

**Windows (Visual Studio):**
```cmd
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON ..
cmake --build . --config Release
```

**Linux/macOS:**
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON ..
cmake --build . --parallel
```

### Run individual tests:

**Windows:**
```cmd
.\build\tests\Release\test_window_manager.exe
.\build\tests\Release\test_input_handler.exe
.\build\tests\Release\test_font.exe
.\build\tests\Release\test_renderer.exe
.\build\tests\Release\test_debug_overlay.exe
```

**Linux/macOS:**
```bash
./build/tests/test_window_manager
./build/tests/test_input_handler
./build/tests/test_font
./build/tests/test_renderer
./build/tests/test_debug_overlay
```

### Run all tests:

```bash
cmake --build build --target run_tests
```

## Writing Tests

Example test case:

```cpp
#include "test_framework.h"
#include "module/my_class.h"

TEST_CASE(TestMyFunction) {
    MyClass obj;
    
    ASSERT_TRUE(obj.Initialize());
    ASSERT_EQ(obj.GetValue(), 42);
}

TEST_MAIN()
```

## Test Modules

### window/test_window_manager.cpp
Tests window creation, lifecycle, framebuffer operations, and GLFW integration.

### input/test_input_handler.cpp
Tests keyboard and mouse input tracking, key state queries, and event handling.

### render/test_font.cpp
Tests bitmap font rendering, character dimensions, and text drawing.

### render/test_renderer.cpp
Tests basic rendering operations, viewport management, and 2D rendering mode.

### debug/test_debug_overlay.cpp
Tests debug overlay visibility, FPS calculation, and error/warning recording.

## Notes

- Most rendering tests require an OpenGL context to fully validate
- Tests without a GL context will verify that functions don't crash
- Input tests simulate callbacks without requiring a window
- Window tests create actual GLFW windows (may briefly appear on screen)
