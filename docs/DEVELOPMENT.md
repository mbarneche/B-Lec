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
