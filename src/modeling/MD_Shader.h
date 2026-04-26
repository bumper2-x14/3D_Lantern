#ifndef MD_SHADER_H
#define MD_SHADER_H

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "math/mat4.h"
#include "math/vec2.h"
#include "math/vec3.h"

/// @brief Wrapper around an OpenGL shader program (vertex + fragment).
///        Handles compilation and sending uniforms.
class MD_Shader {
public:
    /// @brief Compiles shader from given vertex and fragment shader files.
    /// @param vertex_path Path to vertex shader file.
    /// @param fragment_path Path to fragment shader file.
    MD_Shader(const std::string &vertex_path, 
                        const std::string &fragment_path);

    /// @brief Activates the shader program (glUseProgram).
    void apply();

    /// @brief Sends a float uniform to the shader.
    void setFloat(const std::string &name, float value) const;

    /// @brief Sends an int uniform to the shader.
    void setInt(const std::string &name, int value) const;

    /// @brief Sends a bool uniform to the shader.
    void setBool(const std::string &name, bool value) const;

    /// @brief Sends a vec2 uniform to the shader.
    void setVec2(const std::string &name, const Vec2f &value) const;

    /// @brief Sends a vec3 uniform to the shader.
    void setVec3(const std::string &name, const Vec3f &value) const;

    /// @brief Sends a mat4 uniform to the shader.
    void setMat4(const std::string &name, const Mat4f &value) const;
    
    //getter

    /// @brief Returns the OpenGL program id.
    unsigned int get_program_id()const;

    //test

    /// @brief Runs simple tests to check shader compilation.
    static void regression_test();
    
private:
    unsigned int program_id=0; ///< OpenGL program ID.
};

#endif