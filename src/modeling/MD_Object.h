#ifndef MD_OBJECT_H
#define MD_OBJECT_H

#include "MD_Shape.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Material.h"
#include "modeling/MD_Texture.h"
#include "math/TRS_data.h"

class MD_Object{
    public:
        TRSDataf trs; 

        MD_Object(MD_Shape* shape = nullptr, MD_Material* material = nullptr,
                    const TRSDataf& trs_data = {});
        
       void draw(MD_Shader& shader);

        
    private:
        Vec3f* lightColor; 
        MD_Shape* shape;
        MD_Material* material;
};

#endif
