# Core

## Overview
The core module owns application startup, the main loop, and system wiring. It creates the window, initializes rendering and gameplay systems, and drives per-frame input, update, and render calls.

## Components
- Application: initializes GLFW, OpenGL context, and core systems, then runs the loop.
- Main entry point: constructs Application, calls Initialize, then Run.
- ProcessInput: polls input, handles hotbar selection, menu toggle, camera movement, and block interaction.
- Update/Render: advances camera, world, and UI, then renders world and UI.

## Usage
- Create an Application, call Initialize, then Run.
- Use RequestClose to stop the loop cleanly.
- When adding new systems, wire them in Initialize and Update/Render.

## Pitfalls
- Framebuffer resize callback is empty, so viewport/aspect are not updated.
- Camera movement uses a fixed 0.016s delta instead of the real frame delta.
- Scroll wheel block cycling is a placeholder with no callback.
- OpenGL context is set to 4.6 core; glad must load after context creation.
