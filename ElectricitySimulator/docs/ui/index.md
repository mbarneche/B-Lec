# UI

## Overview
The UI module renders a lightweight HUD, including a debug overlay, block palette, and transient status messages. Rendering calls are placeholders.

## Components
- UISystem: tracks window size, debug overlay state, selected block string, and status messages.
- Render: draws debug overlay, block palette, and status messages (conceptually).
- AddStatusMessage: queues messages with a lifetime and fade-out.

## Usage
- Call Initialize with window size during startup.
- Update each frame to age status messages.
- SetCameraPosition and SetSelectedBlock from the game loop.
- ToggleDebugOverlay to show development info.

## Pitfalls
- RenderText and RenderRect are placeholders and do not draw anything yet.
- Coordinates are pixel-based with no scaling for DPI or resize.
