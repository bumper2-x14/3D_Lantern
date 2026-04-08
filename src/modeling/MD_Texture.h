#ifndef MD_TEXTURE
#define MD_TEXTURE 

#include <string.h>
#include <glad/glad.h>
#include "modeling/MD_Shader.h"
#include "assets/image.h"
#include "glad/glad.h"
#include "MD_Material.h"

class MD_Texture:public MD_Material{
    public:
        MD_Texture(const std::string& src);
        void init(int _option_wrap = 0);
        void load_Image();
        int getTextureId();       
        //add u and v on a VBO 
        //using our shaders for drawing
    private:
        //unsigned char* image_Data=nullptr;
        unsigned int texture_Id=0;
        Image data ;
};




#endif
