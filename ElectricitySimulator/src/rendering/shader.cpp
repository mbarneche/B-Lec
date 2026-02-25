/**
 * @file shader.cpp
 * @brief Shader implementation
 */

#include "rendering/shader.h"
#include <glad/glad.h>
#include <iostream>

Shader::Shader(const std::string& vertex_src, const std::string& fragment_src) {
    uint32_t vertex = CompileShader(vertex_src, GL_VERTEX_SHADER);
    uint32_t fragment = CompileShader(fragment_src, GL_FRAGMENT_SHADER);

    program_id_ = glCreateProgram();
    glAttachShader(program_id_, vertex);
    glAttachShader(program_id_, fragment);
    glLinkProgram(program_id_);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id_, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    if (program_id_ != 0) {
        glDeleteProgram(program_id_);
    }
}

void Shader::Use() const {
    glUseProgram(program_id_);
}

void Shader::SetMatrix4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& vector) const {
    glUniform3fv(glGetUniformLocation(program_id_, name.c_str()), 1, &vector[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program_id_, name.c_str()), x, y, z);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int32_t value) const {
    glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
}

uint32_t Shader::CompileShader(const std::string& source, uint32_t shader_type) {
    uint32_t shader = glCreateShader(shader_type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }

    return shader;
}
