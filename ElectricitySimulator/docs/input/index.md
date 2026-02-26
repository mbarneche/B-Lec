# Input

## Overview
The input module wraps GLFW input polling and tracks current and previous states for keys and mouse buttons. It also exposes mouse position and delta.

## Components
- Key enum: small set of gameplay keys (WASD, space, shift, E, Q, ESC).
- InputManager: caches current/previous key and mouse states and computes mouse delta.
- Utility methods: SetMousePosition and CenterMouse for recentering.

## Usage
- Create InputManager with a valid GLFWwindow pointer.
- Call Update once per frame before querying input.
- Use IsKeyJustPressed for edge-triggered actions and IsKeyPressed for held actions.

## Pitfalls
- The arrays are sized to the Key enum; any new keys require resizing.
- Mouse delta is based on frame-to-frame cursor position, so SetMousePosition can spike deltas.
- No scroll wheel or text input handling is provided.
