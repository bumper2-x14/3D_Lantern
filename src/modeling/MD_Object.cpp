#include <glad/glad.h>
#include "MD_Object.h"
#include "math/TRS_data.h"

MD_Object::MD_Object(MD_Shape* _shape, MD_Material* _material, const TRSDataf& trsData)
    : shape(_shape), material(_material), trs(trsData)
{}

Mat4f MD_Object::getTransformMatrix() const {
    return Mat4f::TRS(trs.translation, trs.rotation, trs.scale);
}


void MD_Object::draw(MD_Shader& shader) {
    if (!shape) return;

    const MD_Mesh* mesh = shape->getMesh();
    glBindVertexArray(mesh->VAO);

    // to be fixed
    if (material) {
        if (material->getTexture()){
            glActiveTexture(GL_TEXTURE0);
            //need to modified for multiple textures 
            shader.setInt("texture0", 0);
            glBindTexture(GL_TEXTURE_2D, material->getTextureId());
        }
        else{
            if(!material->getDiffuse() && 
                    !material->getAmbient() &&
                            !material->getSpec())
                return;
            else{
                shader.setBool("texture_Use", false);
                shader.setVec3("ambient", *(material->getAmbient()));
                shader.setVec3("diffuse", *(material->getDiffuse()));
                shader.setVec3("spec", *(material->getSpec()));
            }   
        }
    }
    else{
        //nessecry commands to only draw the mesh
        shader.setBool("outlines" , true );
    }

    // To be fixed
    glDrawElements(GL_TRIANGLES,
                   (GLsizei)mesh->data->indices.size(),
                   GL_UNSIGNED_INT, 0);
}
