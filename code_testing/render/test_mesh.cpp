// code_testing/render/test_mesh.cpp
// Unit tests for Mesh class
// Tests cube creation and culling state

#include "../test_framework.h"
#include "render/mesh.h"

// ============================================================================
// TEST SUITE: Mesh Creation and Basic Properties
// ============================================================================

TEST_CASE(TestMeshCreateCubeCounts) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();

    ASSERT_EQ(cube.GetVertexCount(), 24u);
    ASSERT_EQ(cube.GetIndexCount(), 36u);
}

TEST_CASE(TestMeshBackfaceCullingDefault) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();
    ASSERT_FALSE(cube.IsBackfaceCullingEnabled());
}

TEST_CASE(TestMeshCullingToggle) {
    blec::render::Mesh cube = blec::render::Mesh::CreateCube();

    cube.SetBackfaceCulling(true);
    ASSERT_TRUE(cube.IsBackfaceCullingEnabled());

    cube.SetBackfaceCulling(false);
    ASSERT_FALSE(cube.IsBackfaceCullingEnabled());
}

TEST_CASE(TestMeshMultipleCubes) {
    blec::render::Mesh cube1 = blec::render::Mesh::CreateCube();
    blec::render::Mesh cube2 = blec::render::Mesh::CreateCube();

    ASSERT_EQ(cube1.GetVertexCount(), cube2.GetVertexCount());
    ASSERT_EQ(cube1.GetIndexCount(), cube2.GetIndexCount());
}

TEST_MAIN()
