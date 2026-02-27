# UI Module

## Purpose
Renders the crosshair and pause menu and manages pause state.

## Key Files
- include/ui/ui_manager.h
- src/ui/ui_manager.cpp

## Responsibilities
- Render crosshair in screen center
- Show pause menu and handle button clicks
- Track pause state and screen dimensions

## Usage Notes
- Call `Initialize()` with window dimensions
- Call `UpdateScreenDimensions()` on resize

## Tests
- code_testing/ui/test_ui_manager.cpp
