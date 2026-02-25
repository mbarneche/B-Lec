# B-Lec - Electricity Simulator

A free, open-source 3D electricity simulator game for Windows, Linux, and macOS. Inspired by Minecraft's redstone mechanics, B-Lec allows players to design and test electrical circuits in real-time.

## Features

### Current (Phase 1)
- ✅ 3D block-based world rendering with OpenGL 4.6
- ✅ Infinite flat world generation
- ✅ Camera system with free movement
- ✅ Input handling (keyboard and mouse)
- ✅ Multiple block types with different properties
- ✅ Basic UI system with block selection
- ✅ World serialization (save/load)
- ✅ Cross-platform support (Windows, Linux, macOS)

### Planned Features
- ⏳ Power propagation and circuit simulation
- ⏳ Block placement/destruction mechanics
- ⏳ Complex circuit components (gates, sensors, etc.)
- ⏳ Advanced UI with property editor
- ⏳ WorldEdit-style tools (copy, paste, fill)
- ⏳ Custom textures and block appearance
- ⏳ Multiplayer support
- ⏳ Circuit sharing and community library

## Quick Start

### Prerequisites

- CMake 3.20+
- C++17 compiler (MSVC, GCC, Clang)
- Modern graphics card supporting OpenGL 4.6

### Building

**Linux/macOS:**
```bash
git clone https://github.com/yourusername/B-Lec.git
cd B-Lec
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel $(nproc)
./bin/blec
```

**Windows (Visual Studio):**
```cmd
git clone https://github.com/yourusername/B-Lec.git
cd B-Lec
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
.\bin\Release\blec.exe
```

### Controls

- **WASD**: Move camera forward/backward and left/right
- **Space**: Move camera up
- **Shift**: Move camera down
- **ESC**: Quit application
- **F3**: Toggle debug overlay (planned)
- **E**: Open block palette (planned)

## Project Structure

```
B-Lec/
├── ElectricitySimulator/
│   ├── include/        # Header files for each system
│   ├── src/           # Implementation files
│   ├── assets/        # Game assets (textures, models)
│   └── external/      # External libraries
├── docs/              # Documentation
├── tests/             # Unit tests (planned)
└── CMakeLists.txt     # Build configuration
```

## Architecture

B-Lec uses a **System-Based Architecture** with independent systems managing:

- **Rendering System**: 3D graphics with OpenGL
- **World System**: Block management, chunks, infinite world
- **Input System**: Keyboard and mouse input
- **Camera System**: First-person camera control
- **UI System**: On-screen interface and HUD
- **Persistence System**: Save/load functionality

See [Architecture Documentation](docs/ARCHITECTURE.md) for detailed design information.

## Dependencies

### Minimal External Dependencies
- **GLFW 3.3+**: Window creation and input
- **OpenGL 4.6**: Rendering
- **GLM**: Vector/matrix mathematics
- **GLAD**: OpenGL function loading

All dependencies are lightweight, widely supported, and chosen for maintainability.

## Development

See [Development Guide](docs/DEVELOPMENT.md) for:
- Build instructions for your platform
- Code style guidelines
- How to add new features
- Testing procedures

## Design Principles

### Code Quality
- Clean, organized code following C++ best practices
- Comprehensive documentation and comments
- Minimal dependencies for easy maintenance
- Cross-platform compatibility

### Performance
- Efficient chunk-based world management
- Optimized rendering pipeline
- Support for large circuits without lag
- Scalable to thousands of blocks

### Security & Privacy
- No data collection or telemetry
- No network communication (single-player only)
- All saves stored locally
- Full source code transparency

### Community
- Open-source under MIT License
- Welcoming contribution guidelines
- Active development roadmap
- Regular updates and improvements

## System Requirements

### Minimum
- **OS**: Windows 7+, Ubuntu 16.04+, macOS 10.13+
- **CPU**: 2 cores @ 1.5 GHz
- **RAM**: 512 MB
- **VRAM**: 256 MB with OpenGL 4.6 support
- **Storage**: 50 MB

### Recommended
- **OS**: Windows 10+, Ubuntu 20.04+, macOS 11+
- **CPU**: 4 cores @ 2.0 GHz
- **RAM**: 4 GB
- **VRAM**: 2 GB with OpenGL 4.6 support
- **Storage**: 100 MB

## License

B-Lec is released under the **MIT License**. See [LICENSE](LICENSE) file for details.

The software is free to use, modify, and distribute.

## Contributing

We welcome contributions! Please read [Development Guide](docs/DEVELOPMENT.md) for guidelines.

Steps to contribute:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## Roadmap

### Phase 1 (Current) ✅
- Core world rendering
- Basic block system
- Camera and input
- Persistence

### Phase 2 (Next)
- Power simulation engine
- Block interaction (placement/destruction)
- Circuit components (switches, sensors, etc.)
- Advanced block types

### Phase 3
- UI improvements
- WorldEdit-style tools
- Sound effects
- Performance optimizations

### Phase 4+
- Multiplayer support
- Custom content system
- Community library
- Advanced features

## Credits

B-Lec is created and maintained by the open-source community.

### Inspiration
- Minecraft Redstone mechanics
- Real-world electrical circuit principles
- Educational simulation games

### Libraries Used
- GLFW (Graphics)
- OpenGL (Rendering)
- GLM (Mathematics)

## Support

For issues, questions, or suggestions:
- Open an issue on GitHub
- Check documentation in [docs/](docs/)
- Review code examples in the repository

## Disclaimer

This software is provided "as is" without warranty. Users are responsible for their own use of the software.

## Vision

B-Lec aims to be the premier educational and recreational electricity simulation game, combining:
- Intuitive gameplay with Minecraft-like mechanics
- Accurate simulation of electrical concepts
- Accessible to both casual players and educators
- Open-source for transparency and community contribution

---

**Happy circuit building!** 🔌⚡

For more information, see the [documentation](docs/) or visit our GitHub repository.
