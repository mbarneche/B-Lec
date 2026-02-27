# Input Module

## Purpose
Tracks keyboard and mouse input state via GLFW callbacks.

## Key Files
- include/input/input_handler.h
- src/input/input_handler.cpp

## Responsibilities
- Register GLFW input callbacks
- Track key and mouse button states
- Provide mouse position and per-frame delta

## Usage Notes
- Call `RegisterCallbacks()` after the window is created
- Call `ResetMouseDelta()` once per frame after consuming mouse input

## Tests
- code_testing/input/test_input_handler.cpp
