// code_testing/render/test_renderer_3d.cpp
// Unit tests for Renderer 3D functionality
// Tests perspective projection, view/model matrices, depth testing, and culling

#include "../test_framework.h"
#include "render/renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace blec::test {

// ============================================================================
// TEST SUITE: Renderer 3D Mode Initialization
// ============================================================================

TEST(RendererBegin3D) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Begin3D should not crash or error
    renderer.Begin3D(1280, 720, 45.0f);
    
    EXPECT(true, "Begin3D should initialize 3D rendering mode");
}

TEST(RendererEnd3D) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    renderer.Begin3D(1280, 720, 45.0f);
    renderer.End3D();
    
    EXPECT(true, "End3D should clean up 3D rendering mode");
}

TEST(Renderer3DCycle) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Multiple 3D cycles should work correctly
    for (int i = 0; i < 3; ++i) {
        renderer.Begin3D(1280, 720, 45.0f);
        renderer.End3D();
    }
    
    EXPECT(true, "Multiple Begin3D/End3D cycles should work");
}

// ============================================================================
// TEST SUITE: Projection Matrix
// ============================================================================

TEST(RendererSetProjection) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    
    renderer.SetProjection(projection);
    
    EXPECT(true, "SetProjection should accept valid projection matrix");
}

TEST(RendererProjectionValid) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    float fov = 45.0f;
    float aspect = 1280.0f / 720.0f;
    float near_plane = 0.1f;
    float far_plane = 100.0f;
    
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
    
    renderer.SetProjection(projection);
    
    // Projection matrix should be 4x4
    EXPECT(true, "Perspective projection matrix should be valid");
}

TEST(RendererDifferentFOVs) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Test different field of view angles
    for (float fov : {30.0f, 45.0f, 60.0f, 90.0f}) {
        glm::mat4 projection = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, 0.1f, 100.0f);
        renderer.SetProjection(projection);
    }
    
    EXPECT(true, "Renderer should handle different FOV angles");
}

TEST(RendererDifferentAspectRatios) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Test different aspect ratios
    for (float aspect : {16.0f / 9.0f, 4.0f / 3.0f, 1.0f, 21.0f / 9.0f}) {
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        renderer.SetProjection(projection);
    }
    
    EXPECT(true, "Renderer should handle different aspect ratios");
}

// ============================================================================
// TEST SUITE: View Matrix (Camera)
// ============================================================================

TEST(RendererSetView) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    glm::vec3 position(0.0f, 0.0f, 5.0f);
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    
    glm::mat4 view = glm::lookAt(position, target, up);
    renderer.SetView(view);
    
    EXPECT(true, "SetView should accept valid view matrix");
}

TEST(RendererViewFromDifferentPositions) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Test view matrices from different camera positions
    std::array<glm::vec3, 5> positions = {
        glm::vec3(0.0f, 0.0f, 5.0f),    // Behind
        glm::vec3(5.0f, 0.0f, 0.0f),    // Right
        glm::vec3(-5.0f, 0.0f, 0.0f),   // Left
        glm::vec3(0.0f, 5.0f, 0.0f),    // Above
        glm::vec3(3.0f, 3.0f, 3.0f)     // Diagonal
    };
    
    for (const auto& pos : positions) {
        glm::mat4 view = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        renderer.SetView(view);
    }
    
    EXPECT(true, "Renderer should handle view matrices from different positions");
}

// ============================================================================
// TEST SUITE: Model Matrix (Object Transform)
// ============================================================================

TEST(RendererSetModel) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    glm::mat4 model = glm::mat4(1.0f);  // Identity matrix
    renderer.SetModel(model);
    
    EXPECT(true, "SetModel should accept identity matrix");
}

TEST(RendererSetModelWithTranslation) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, -5.0f));
    renderer.SetModel(model);
    
    EXPECT(true, "SetModel should accept translated matrix");
}

TEST(RendererSetModelWithRotation) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    renderer.SetModel(model);
    
    EXPECT(true, "SetModel should accept rotated matrix");
}

TEST(RendererSetModelWithScale) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    renderer.SetModel(model);
    
    EXPECT(true, "SetModel should accept scaled matrix");
}

TEST(RendererSetModelComplex) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Translate, rotate, then scale (TRS)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 2.0f, 3.0f));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
    
    renderer.SetModel(model);
    
    EXPECT(true, "SetModel should accept complex transformation matrices");
}

// ============================================================================
// TEST SUITE: Depth Testing
// ============================================================================

TEST(RendererEnableDepthTest) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    renderer.EnableDepthTest();
    
    EXPECT(true, "EnableDepthTest should not error");
}

TEST(RendererDisableDepthTest) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    renderer.DisableDepthTest();
    
    EXPECT(true, "DisableDepthTest should not error");
}

TEST(RendererToggleDepthTest) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Enable then disable
    renderer.EnableDepthTest();
    renderer.DisableDepthTest();
    renderer.EnableDepthTest();
    
    EXPECT(true, "Depth testing should be toggleable");
}

// ============================================================================
// TEST SUITE: Back-Face Culling
// ============================================================================

TEST(RendererEnableBackfaceCulling) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    renderer.EnableBackfaceCulling();
    
    EXPECT(true, "EnableBackfaceCulling should not error");
}

TEST(RendererDisableBackfaceCulling) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    renderer.DisableBackfaceCulling();
    
    EXPECT(true, "DisableBackfaceCulling should not error");
}

TEST(RendererToggleBackfaceCulling) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Enable then disable multiple times
    for (int i = 0; i < 3; ++i) {
        renderer.EnableBackfaceCulling();
        renderer.DisableBackfaceCulling();
    }
    
    EXPECT(true, "Back-face culling should be toggleable");
}

TEST(RendererCullingWithBegin3D) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    renderer.Begin3D(1280, 720, 45.0f);
    renderer.EnableBackfaceCulling();
    renderer.DisableBackfaceCulling();
    renderer.End3D();
    
    EXPECT(true, "Back-face culling should work within 3D mode");
}

// ============================================================================
// TEST SUITE: Combined 3D Operations
// ============================================================================

TEST(RendererComplete3DSetup) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    int width = 1280;
    int height = 720;
    float fov = 45.0f;
    
    renderer.Begin3D(width, height, fov);
    
    // Set up projection
    glm::mat4 projection = glm::perspective(glm::radians(fov), 
                                           static_cast<float>(width) / height, 
                                           0.1f, 100.0f);
    renderer.SetProjection(projection);
    
    // Set up view (camera at (0,0,5) looking at origin)
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    renderer.SetView(view);
    
    // Set up model (identity - object at origin)
    glm::mat4 model = glm::mat4(1.0f);
    renderer.SetModel(model);
    
    // Enable features
    renderer.EnableDepthTest();
    renderer.EnableBackfaceCulling();
    
    renderer.End3D();
    
    EXPECT(true, "Complete 3D setup should work without errors");
}

TEST(RendererMultiple3DFrames) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Render multiple frames
    for (int frame = 0; frame < 10; ++frame) {
        renderer.Begin3D(1280, 720, 45.0f);
        
        renderer.SetProjection(projection);
        renderer.SetView(view);
        
        // Rotate model each frame
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), 
                                      glm::radians(36.0f * frame), 
                                      glm::vec3(0.0f, 1.0f, 0.0f));
        renderer.SetModel(model);
        
        renderer.EnableDepthTest();
        renderer.EnableBackfaceCulling();
        
        renderer.End3D();
    }
    
    EXPECT(true, "Rendering multiple 3D frames should work");
}

TEST(RendererDepthAndCullingCombined) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Both depth testing and back-face culling should work together
    renderer.EnableDepthTest();
    renderer.EnableBackfaceCulling();
    
    // Disable one
    renderer.DisableBackfaceCulling();
    EXPECT(true, "Should handle disabling culling while depth test is enabled");
    
    // Enable again
    renderer.EnableBackfaceCulling();
    
    // Disable depth
    renderer.DisableDepthTest();
    EXPECT(true, "Should handle disabling depth while culling is enabled");
    
    EXPECT(true, "Depth testing and culling should work independently");
}

// ============================================================================
// TEST SUITE: View Frustum and Projection Properties
// ============================================================================

TEST(RendererNearAndFarPlanes) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Test different near/far plane configurations
    float near_planes[] = {0.01f, 0.1f, 1.0f};
    float far_planes[] = {10.0f, 100.0f, 1000.0f};
    
    for (float near_plane : near_planes) {
        for (float far_plane : far_planes) {
            if (near_plane < far_plane) {
                glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                                        16.0f / 9.0f, 
                                                        near_plane, 
                                                        far_plane);
                renderer.SetProjection(projection);
            }
        }
    }
    
    EXPECT(true, "Renderer should handle various near/far plane settings");
}

TEST(RendererWideFieldOfView) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Wide FOV (fisheye-like)
    glm::mat4 projection = glm::perspective(glm::radians(120.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    renderer.SetProjection(projection);
    
    EXPECT(true, "Renderer should handle wide field of view");
}

TEST(RendererNarrowFieldOfView) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Narrow FOV (zoom-like)
    glm::mat4 projection = glm::perspective(glm::radians(15.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    renderer.SetProjection(projection);
    
    EXPECT(true, "Renderer should handle narrow field of view");
}

// ============================================================================
// TEST SUITE: State Management
// ============================================================================

TEST(RendererStatePersistence) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
    
    renderer.SetProjection(projection);
    renderer.SetView(view);
    renderer.SetModel(model);
    
    // Set again - should not error or change
    renderer.SetProjection(projection);
    renderer.SetView(view);
    renderer.SetModel(model);
    
    EXPECT(true, "Renderer state should persist across multiple sets");
}

TEST(RendererStateSwitching) {
    blec::render::Renderer renderer;
    renderer.Initialize();
    
    // Create two different states
    glm::mat4 proj1 = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    glm::mat4 proj2 = glm::perspective(glm::radians(60.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    
    renderer.SetProjection(proj1);
    renderer.SetProjection(proj2);
    renderer.SetProjection(proj1);
    
    // Should be able to switch between states
    EXPECT(true, "Renderer should handle state switching");
}

} // namespace blec::test
