# Window Module

## Purpose
Manages GLFW lifecycle, window creation, and basic window operations.

## Key Files
- include/window/window_manager.h
- src/window/window_manager.cpp

## Responsibilities
- Initialize and terminate GLFW
- Create and destroy the window
- Provide framebuffer size and swap buffers
- Expose the GLFW window handle for callbacks

## Usage Notes
- Call `InitializeGLFW()` before creating a window
- Call `Shutdown()` on exit (safe to call multiple times)

## Tests
- code_testing/window/test_window_manager.cpp
