#ifndef MD_LIGHT_H
#define MD_LIGHT_H 

#include "math/vec3.h"
#include "math/transform.h"
#include "modeling/MD_Shader.h"


class MD_Light{
    public: 
       
        MD_Light();
        MD_Light(const Vec3f _Pos,const Vec3f _lightColor);
        MD_Light(const Vec3f& _lightColor);
        
        void drawLightSource(MD_Shader& shader);
        
        void applyLight(MD_Shader& shader);
        
    private:

        Vec3f& lightColor;
        Transform lightTransfrom;
};




#endif
