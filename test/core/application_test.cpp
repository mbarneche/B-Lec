#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/core/application.h"
#include <GLFW/glfw3.h>

TEST(ApplicationTest, InitializeRunAndClose) {
    Application app;
    ASSERT_TRUE(app.Initialize());

    GLFWwindow* window = app.GetWindow();
    ASSERT_NE(window, nullptr);

    glfwSetWindowShouldClose(window, GLFW_TRUE);
    app.Run();

    EXPECT_FALSE(app.IsRunning());
}
