# Rendering

## Overview
The rendering module provides a simple OpenGL renderer that draws blocks as cubes using a basic shader. It relies on GLAD for OpenGL function loading.

## Components
- Renderer: initializes GLAD, sets depth/cull state, and renders all blocks each frame.
- Shader: compiles and links GLSL vertex/fragment shaders and exposes uniform setters.
- Cube mesh: a static VAO/VBO/EBO storing cube positions with indexed drawing.

## Usage
- Call Renderer::Initialize after creating the OpenGL context.
- Each frame, call Clear then Render(world, camera).
- Use GetShader if you need to set additional uniforms.

## Pitfalls
- The current shader expects a vertex color attribute, but only position data is provided.
- Block colors are computed but not sent to the shader.
- No batching or mesh generation is used; all blocks are drawn one-by-one.
- Viewport resizing is not handled here; caller must call SetViewport.
