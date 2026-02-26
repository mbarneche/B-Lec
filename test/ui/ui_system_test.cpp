#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/ui/ui_system.h"

TEST(UISystemTest, StateAndMessages) {
    UISystem ui;
    ui.Initialize(800, 600);

    EXPECT_FALSE(ui.IsDebugOverlayEnabled());
    ui.ToggleDebugOverlay();
    EXPECT_TRUE(ui.IsDebugOverlayEnabled());

    ui.SetShowBlockPalette(false);
    EXPECT_FALSE(ui.IsBlockPaletteShown());

    ui.SetCameraPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    ui.SetSelectedBlock("Copper Wire");

    ui.AddStatusMessage("hello", 0.1f);
    ui.Update(0.05f);
    ui.Render();
    ui.Update(0.1f);
    ui.Render();
}
