/**
 * @file shader.h
 * @brief Shader management and compilation
 * 
 * Handles compilation and linking of GLSL shaders.
 */

#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>

/**
 * @class Shader
 * @brief Represents an OpenGL shader program
 */
class Shader {
public:
    /**
     * @brief Constructor
     * @param vertex_src Vertex shader source code
     * @param fragment_src Fragment shader source code
     */
    Shader(const std::string& vertex_src, const std::string& fragment_src);

    /**
     * @brief Destructor
     */
    ~Shader();

    /**
     * @brief Use this shader program
     */
    void Use() const;

    /**
     * @brief Set a 4x4 matrix uniform
     * @param name Name of the uniform
     * @param matrix The matrix value
     */
    void SetMatrix4(const std::string& name, const glm::mat4& matrix) const;

    /**
     * @brief Set a 3D vector uniform
     * @param name Name of the uniform
     * @param vector The vector value
     */
    void SetVec3(const std::string& name, const glm::vec3& vector) const;

    /**
     * @brief Set a 3D vector uniform
     * @param name Name of the uniform
     * @param x X component
     * @param y Y component
     * @param z Z component
     */
    void SetVec3(const std::string& name, float x, float y, float z) const;

    /**
     * @brief Set a float uniform
     * @param name Name of the uniform
     * @param value The float value
     */
    void SetFloat(const std::string& name, float value) const;

    /**
     * @brief Set an integer uniform
     * @param name Name of the uniform
     * @param value The integer value
     */
    void SetInt(const std::string& name, int32_t value) const;

    /**
     * @brief Get the OpenGL program ID
     * @return The program ID
     */
    uint32_t GetProgramID() const { return program_id_; }

private:
    /**
     * @brief Compile a shader
     * @param source Source code of the shader
     * @param shader_type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
     * @return The compiled shader object ID
     */
    static uint32_t CompileShader(const std::string& source, uint32_t shader_type);

private:
    uint32_t program_id_;
};
