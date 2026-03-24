#ifndef MD_TEXTURE
#define MD_TEXTURE 

#include <string.h>
#include <glad/glad.h>
#include "modeling/MD_Shader.h"
#include "../external/stb_image.h"

class MD_Texture{
    public:
        MD_Texture(const std::string& src, MD_Shader);
        MD_Texture(const std::string& src, int _option_wrap);
        MD_Texture(const std::string& src, int _option_wrap,
                         int _option_min, int _option_max);
        
        //add u and v on a VBO 
        //using our shaders for drawing
    private:
        //unsigned char* image_Data=nullptr;
        unsigned int texture_Id=0;
};



#endif
