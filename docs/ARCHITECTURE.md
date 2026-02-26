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
│  │  │  Font    │ │ OpenGL   │ │           │
│  │  └──────────┘ └──────────┘ │           │
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

**Purpose**: Provides rendering primitives and text display.

**Key Classes**:
- `Renderer`: Basic OpenGL rendering operations
- `BitmapFont`: 5x7 pixel font for text rendering

**Key Features**:
- Viewport management
- 2D screen-space rendering
- Color and blending control
- Filled rectangle drawing
- ASCII character rendering
- Multi-line text support

**Dependencies**:
- OpenGL (via GLFW)

**Font Data**:
- Public domain 5x7 pixel bitmap font
- Supports ASCII 32-126 (printable characters)
- Scalable rendering

### Debug Module (`include/debug/`, `src/debug/`)

**Purpose**: Displays real-time development information.

**Key Classes**:
- `DebugOverlay`: Manages debug state and rendering

**Key Features**:
- Toggleable overlay (F12 key)
- FPS calculation and display
- Input state display (keys pressed, mouse position/delta)
- Error and warning tracking
- Semi-transparent background box

**Dependencies**:
- Input module (for state queries)
- Render module (for drawing)

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
  │   ├── renderer.h            # Rendering operations
  │   └── font.h                # Bitmap font rendering
  └── debug/
      └── debug_overlay.h       # Debug overlay interface

src/
  ├── window/
  │   └── window_manager.cpp    # Window implementation
  ├── input/
  │   └── input_handler.cpp     # Input implementation
  ├── render/
  │   ├── renderer.cpp          # Rendering implementation
  │   └── font.cpp              # Font implementation + data
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
  │   └── test_renderer.cpp
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
