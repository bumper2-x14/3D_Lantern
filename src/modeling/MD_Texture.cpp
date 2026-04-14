#include "MD_Texture.h"

MD_Texture::MD_Texture(const std::string& src){
    data = std::move(Image::load(src));
    glGenTextures(1, &texture_Id);
    glBindTexture(GL_TEXTURE_2D, texture_Id);
}


MD_Texture::MD_Texture()

/*
DEV NOTE:
No clear def of _option_wrap, use an enum instead
*/
void MD_Texture::init(int _option_wrap){
    glBindTexture(GL_TEXTURE_2D, texture_Id);
    switch(_option_wrap){
         case 0: 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                              GL_MIRRORED_REPEAT);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                              GL_MIRRORED_REPEAT);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                        GL_LINEAR_MIPMAP_LINEAR);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                                    GL_LINEAR);
            break;

        case 1:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                           GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                           GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                        GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                            GL_LINEAR);
            break;

        case 2:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                           GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                           GL_MIRRORED_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                        GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                                    GL_LINEAR);
            break;

        case 3:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                           GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                           GL_CLAMP_TO_EDGE);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                        GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                                    GL_LINEAR);
            break;

        case 4:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                           GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                           GL_CLAMP_TO_BORDER);
            float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D,
                        GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                        GL_LINEAR_MIPMAP_LINEAR);  
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                                    GL_LINEAR);
            break;
           }  
}

void MD_Texture::loadImage(){
    glBindTexture(GL_TEXTURE_2D, texture_Id);
    glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGBA,
                data.width,
                data.height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data.data
                );
    glGenerateMipmap(GL_TEXTURE_2D);
}


int MD_Texture::getTextureId(){
    return texture_Id;
}
