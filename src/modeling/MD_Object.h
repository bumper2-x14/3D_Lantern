#ifndef MD_OBJECT_H
#define MD_OBJECT_H

#include "MD_Shape.h"
#include "math/transform.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Material.h"
#include "modeling/MD_Texture.h"

class MD_Object{
    public:
        /*
        MD_Object();
        */
       MD_Object(MD_Shape* _shape = nullptr, 
                            Transform* _transform = nullptr,
                                        MD_Material* _material = nullptr );
        void draw(MD_Shader& shader);

        
    private:
        Vec3f* lightColor; 
        MD_Shape* shape;
        MD_Material* material;
        Transform* transform;
};

#endif
