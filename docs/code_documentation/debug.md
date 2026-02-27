# Debug Module

## Purpose
Displays real-time debug information (FPS, input state, camera data, block counts).

## Key Files
- include/debug/debug_overlay.h
- src/debug/debug_overlay.cpp

## Responsibilities
- Track FPS and overlay visibility
- Render text-based debug data
- Provide error and warning counters

## Usage Notes
- Call `Update()` once per frame
- Call `Render()` during 2D rendering phase

## Tests
- code_testing/debug/test_debug_overlay.cpp
