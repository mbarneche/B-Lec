#define private public
#include "../../ElectricitySimulator/include/input/input_manager.h"
#undef private

#include <gtest/gtest.h>
#include "../test_helpers.h"

TEST(InputManagerTest, KeyAndMouseStateTracking) {
    GlfwTestContext context;
    InputManager manager(context.window());

    manager.current_keys_[static_cast<int>(Key::W)] = true;
    manager.previous_keys_[static_cast<int>(Key::W)] = false;
    EXPECT_TRUE(manager.IsKeyPressed(Key::W));
    EXPECT_TRUE(manager.IsKeyJustPressed(Key::W));

    manager.current_keys_[static_cast<int>(Key::W)] = false;
    manager.previous_keys_[static_cast<int>(Key::W)] = true;
    EXPECT_FALSE(manager.IsKeyJustPressed(Key::W));

    manager.current_mouse_buttons_[0] = true;
    manager.previous_mouse_buttons_[0] = false;
    EXPECT_TRUE(manager.IsMouseButtonPressed(0));
    EXPECT_TRUE(manager.IsMouseButtonJustPressed(0));
    EXPECT_FALSE(manager.IsMouseButtonPressed(3));

    glfwSetCursorPos(context.window(), 12.0, 34.0);
    manager.Update();
    glm::vec2 pos = manager.GetMousePosition();
    EXPECT_NEAR(pos.x, 12.0f, 1e-3f);
    EXPECT_NEAR(pos.y, 34.0f, 1e-3f);
}
