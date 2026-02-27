# B-Lec Code Review (2026-02-26)

## Scope
Reviewed the repository excluding build/ and program/. Focused on code quality, documentation, tests, and build/testability.

## Findings (ordered by severity)

### Critical
1. **Several test files cannot compile or link due to mismatched test macros and missing test entry points.**
   - The framework only defines `TEST_CASE`/`ASSERT_*` and `TEST_MAIN` [code_testing/test_framework.h](code_testing/test_framework.h#L66-L116).
   - Multiple tests use `TEST`/`EXPECT` macros that do not exist in the framework: [code_testing/render/test_camera.cpp](code_testing/render/test_camera.cpp#L17-L37), [code_testing/render/test_mesh.cpp](code_testing/render/test_mesh.cpp#L205-L210), [code_testing/render/test_renderer_3d.cpp](code_testing/render/test_renderer_3d.cpp#L17-L36), [code_testing/world/test_block_system.cpp](code_testing/world/test_block_system.cpp#L13-L36).
   - These test files also do not call `TEST_MAIN()`, so they will not provide a `main()` entry point under the current framework.

2. **Tests reference APIs that do not exist in production code.**
   - Camera tests call `GetMovementSpeed()` and `GetRotationSpeed()` [code_testing/render/test_camera.cpp](code_testing/render/test_camera.cpp#L235-L256), but the camera API only exposes setters [include/render/camera.h](include/render/camera.h#L58-L69).
   - Mesh tests call `GetVertexPosition`, `GetVertexColor`, `GetVertexNormal`, and `GetIndex` [code_testing/render/test_mesh.cpp](code_testing/render/test_mesh.cpp#L283-L337), but mesh does not expose these accessors [include/render/mesh.h](include/render/mesh.h#L36-L54).

3. **Camera yaw/pitch unit mismatch causes incorrect debug output and unclear API contract.**
   - Camera stores and updates yaw/pitch in degrees [src/render/camera.cpp](src/render/camera.cpp#L14-L73), but the header claims radians [include/render/camera.h](include/render/camera.h#L58-L62).
   - Debug overlay converts yaw/pitch from radians to degrees again [src/debug/debug_overlay.cpp](src/debug/debug_overlay.cpp#L137-L140), and main passes `GetYaw()`/`GetPitch()` directly [src/main.cpp](src/main.cpp#L208-L211). This double conversion produces incorrect displayed angles.

### High
4. **Movement/rotation speed settings are unused, and `Update()` ignores `deltaTime`.**
   - `movement_speed_` and `rotation_speed_` are set but never applied in movement or rotation [src/render/camera.cpp](src/render/camera.cpp#L14-L78).
   - Main uses hard-coded speed constants instead of the camera settings [src/main.cpp](src/main.cpp#L152-L181).
   - This conflicts with tests that expect speed to affect motion [code_testing/render/test_camera.cpp](code_testing/render/test_camera.cpp#L259-L286).

5. **Build warnings likely under `-Wextra` (and are treated as errors per requirements).**
   - `DebugOverlay::Update` ignores the `input` parameter [src/debug/debug_overlay.cpp](src/debug/debug_overlay.cpp#L36-L46).
   - `Camera::Update` ignores `deltaTime` [src/render/camera.cpp](src/render/camera.cpp#L76-L78).

6. **Back-face culling default contradicts the tests.**
   - Mesh culling defaults to enabled [src/render/mesh.cpp](src/render/mesh.cpp#L11-L12), but tests expect disabled [code_testing/render/test_mesh.cpp](code_testing/render/test_mesh.cpp#L205-L210).

7. **`run_tests` target does not depend on the test executables.**
   - `run_tests` only adds post-build commands; it does not declare dependencies on the test targets [code_testing/CMakeLists.txt](code_testing/CMakeLists.txt#L48-L68). This can result in trying to run tests before they are built.

### Medium
8. **Documentation version mismatches.**
   - Root CMake requires 4.2 [CMakeLists.txt](CMakeLists.txt#L1) but README/BUILDING say 3.20+ [README.md](README.md#L20), [docs/BUILDING.md](docs/BUILDING.md#L34).
   - GLM version differs between README and BUILDING [README.md](README.md#L63-L66), [docs/BUILDING.md](docs/BUILDING.md#L43-L45).

9. **Test documentation is out of date.**
   - The test structure in [code_testing/README.md](code_testing/README.md#L13-L27) omits newer tests (render camera/mesh/renderer_3d, ui, world).

10. **Potential divide-by-zero when the window is minimized.**
   - Projection uses `fb_height` as a divisor without a guard [src/main.cpp](src/main.cpp#L188-L201). If `fb_height` is 0, this is undefined behavior.

### Low
11. **Block system total count recalculation is O(N) per `SetBlock`.**
   - `SetBlock` always calls a full scan [src/world/block_system.cpp](src/world/block_system.cpp#L136-L175). This can be optimized to increment/decrement the count to avoid repeated full traversals.

12. **No dedicated documentation files in docs/code_documentation.**
   - The directory exists but is empty [docs/code_documentation](docs/code_documentation). If the requirement expects external docs per module, this is currently unmet.

## Tests
- **Attempted:** Configure/build via CMake Tools, then list and run tests.
- **Result:** CMake Tools build failed to configure (no tests were run). The initial test discovery returned no tests.
- **Additional risk:** Several tests invoke OpenGL calls without an active context, which can be undefined on some platforms [code_testing/render/test_renderer.cpp](code_testing/render/test_renderer.cpp#L15-L65), [code_testing/render/test_renderer_3d.cpp](code_testing/render/test_renderer_3d.cpp#L17-L120).

## Overall Assessment
The codebase is organized and modular, but the current test suite is inconsistent with the testing framework and the production APIs. This prevents a clean build, blocks test execution, and violates the “warnings-as-errors” rule. Documentation exists but has version mismatches and gaps. Performance and robustness improvements are recommended, particularly around the camera API, block counting, and rendering safety checks.

## Recommended Next Steps
1. Align test framework usage across all tests (`TEST_CASE`/`ASSERT_*`) and add `TEST_MAIN()` where required.
2. Either add the missing API getters used by tests or update tests to match the actual public API.
3. Decide and document the camera angle unit (degrees or radians) and make the code consistent end-to-end.
4. Use `movement_speed_`/`rotation_speed_` in camera movement/rotation and consume `deltaTime` to avoid unused-parameter warnings.
5. Fix the `run_tests` target dependencies so it builds the tests before running them.
6. Update README/BUILDING/test docs to match the current build requirements and test layout.
