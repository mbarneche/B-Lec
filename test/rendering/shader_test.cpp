#include <gtest/gtest.h>
#include "../../ElectricitySimulator/include/rendering/shader.h"
#include "../test_helpers.h"

TEST(ShaderTest, CompileAndUse) {
    GlfwTestContext context;
    glfwMakeContextCurrent(context.window());

    const std::string vertex = "#version 460 core\n"
                               "layout (location = 0) in vec3 position;\n"
                               "void main() { gl_Position = vec4(position, 1.0); }\n";
    const std::string fragment = "#version 460 core\n"
                                 "out vec4 FragColor;\n"
                                 "void main() { FragColor = vec4(1.0); }\n";

    Shader shader(vertex, fragment);
    EXPECT_NE(shader.GetProgramID(), 0u);
    shader.Use();
    shader.SetFloat("uValue", 1.0f);
    shader.SetInt("uInt", 2);
    shader.SetVec3("uVec", 0.1f, 0.2f, 0.3f);
    shader.SetMatrix4("uMat", glm::mat4(1.0f));
}
