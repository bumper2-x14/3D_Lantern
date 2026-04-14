#ifndef MD_MATERIAL_H
#define MD_MATERIAL_H

#include "MD_Texture.h"
#include "math/vec3.h"

class MD_Material{
    public:
        MD_Material(MD_Texture* _texture)

        MD_Material(Vec3f* _ambient = nullptr,
                        Vec3f* _diffuse = nullptr,
                                Vec3f* _spec = nullptr );

        MD_Texture* getTexture(){ return texture;}

        Vec3f getAmbient(){ return ambient; }

        Vec3f getDiffuse(){ return diffuse; }

        Vec3f spec(){ return spec; }

    private:

        Texture* texture = nullptr;

        unsigned int gl_Texture_Id= -1;

        Vec3f* ambient = nullptr;

        Vec3f* diffuse = nullptr;
        
        Vec3f* spec = nullptr;
};

#endif
