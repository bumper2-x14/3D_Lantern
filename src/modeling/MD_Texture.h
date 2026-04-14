#ifndef MD_TEXTURE
#define MD_TEXTURE 

#include <string.h>
#include <glad/glad.h>
#include "modeling/MD_Shader.h"
#include "assets/texture.h"
#include "glad/glad.h"
#include "MD_Material.h"

class MD_Texture{
    public:
        MD_Texture(const std::string& src);
        void init(int _option_wrap = 0);

        void loadImage();

        /*
        getter methods should be const both in header and cpp
        */
        int getTextureId()const;       
        //add u and v on a VBO 
        //using our shaders for drawing
    private:
        unsigned int texture_Id=0;

        Texture* texture;
        //unsigned char* image_Data = nullptr;

};




#endif
