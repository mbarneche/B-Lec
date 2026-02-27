# Render Module

## Purpose
Provides basic 2D and 3D rendering utilities, camera control, and mesh creation.

## Key Files
- include/render/renderer.h
- src/render/renderer.cpp
- include/render/camera.h
- src/render/camera.cpp
- include/render/mesh.h
- src/render/mesh.cpp
- include/render/font.h
- src/render/font.cpp

## Responsibilities
- Manage OpenGL state for 2D and 3D drawing
- Provide a free-flying camera with input-based movement
- Create and render simple meshes (cube)
- Render bitmap text for overlays

## Usage Notes
- Camera rotation values are stored in radians
- `SetMovementSpeed()` and `SetRotationSpeed()` scale queued inputs applied in `Update()`
- Mesh back-face culling defaults to disabled; enable per mesh as needed

## Tests
- code_testing/render/test_renderer.cpp
- code_testing/render/test_renderer_3d.cpp
- code_testing/render/test_camera.cpp
- code_testing/render/test_mesh.cpp
- code_testing/render/test_font.cpp
