# Camera

## Overview
The camera module provides a simple 3D camera with view and projection matrices. It supports basic movement and yaw/pitch rotation.

## Components
- Camera: stores position, target, up vector, and projection parameters (fov, near, far, aspect).
- Movement helpers: MoveForward/MoveRight/MoveUp update position and target together.
- Rotate: updates yaw and pitch with clamping to avoid flips, then recalculates target.

## Usage
- Construct Camera with initial position, target, and up vector.
- Call SetAspectRatio whenever the window size changes.
- Use GetViewMatrix and GetProjectionMatrix each frame for rendering.
- Apply Move* and Rotate based on input.

## Pitfalls
- Update is currently empty; any smoothing or inertia must be added manually.
- Rotation pivots around the camera position, not a separate focus point.
- No mouse input integration is built in; caller must supply deltas.
