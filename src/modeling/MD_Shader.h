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

class MD_Shader {
public:
    //complie shader
    MD_Shader(const std::string &vertex_path, 
                        const std::string &fragment_path);
    //use program shader
    void apply();
    //puting values in unifrom 
    void setFloat(const std::string &name, float value) const;
    void setInt(const std::string &name, int value) const;
    void setBool(const std::string &name, bool value) const;
    void setVec2(const std::string &name, const Vec2f &value);
    void setVec3(const std::string &name, const Vec3f &value);
    void setMat4(const std::string &name, const Mat4f &value);
    
    //getter
    unsigned int get_program_id()const;

    //test
    static void regression_test();
    
  
private:

  
    unsigned int program_id=0;
};

#endif
