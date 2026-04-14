#include "MD_Material.h"

MD_Material::MD_Material(Texture* _texture){
	
	texture = new Texture(_texture);

	glGenTextures(1, &texture_Id);
	
	glBindTexture(GL_TEXTURE_2D, texture_Id);

	
	//opengl texture wraping options 
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                              GL_MIRRORED_REPEAT);
            
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                              GL_MIRRORED_REPEAT);
            
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                        GL_LINEAR_MIPMAP_LINEAR);
            
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                                    GL_LINEAR);
	
}

MD_Material::MD_Material(Vec3f* _ambient, 
				Vec3f* _diffuse,
					Vec3f* _spec){
	ambient = new Vec3f(_ambient);
	diffuse = new Vec3f(_diffuse);
	spec 	= new Vec3f(_spec);
}

