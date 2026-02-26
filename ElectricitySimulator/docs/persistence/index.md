# Persistence

## Overview
The persistence module provides static helpers for saving and loading worlds, and for managing save file paths.

## Components
- WorldSerializer::SaveWorld/LoadWorld: open a binary file for write/read (serialization is a placeholder).
- WorldExists: checks file presence via std::filesystem.
- GetSaveDirectory/GetWorldPath: manage the saves folder and the .blec extension.
- DeleteWorld: removes a saved world file.

## Usage
- Build a path with GetWorldPath and call SaveWorld/LoadWorld.
- Use WorldExists before trying to load a save.

## Pitfalls
- Actual serialization of chunks and blocks is not implemented yet.
- Save directory is relative to the current working directory.
- Error reporting goes to stderr only.
