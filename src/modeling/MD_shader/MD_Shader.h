#ifndef MD_SHADER_H
#define MD_SHADER_H

#include <string>

#include "../../math/vec2.h"
#include "../../math/vec3.h"
#include "../../math/mat4.h"

class MD_Shader {
    public:
        MD_Shader(const std::string& vertex_file, const std::string& fragment_file);

        void apply();

        void setFloat(const std::string& name, float value) const;
        void setInt(const std::string& name, int value) const;
        // note for developer: when using glGetUniformLocation for setBool, value should be casted to int
        void setBool(const std::string& name, bool value) const; 
        void setVec2(const std::string& name, const Vec2& vlaue);
        void setVec3(const std::string& name, const Vec3& vlaue);
        void setMat4(const std::string& name, const Mat4& value);

    private:
        unsigned int program_id;
};

#endif