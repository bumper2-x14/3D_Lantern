#include <glad/glad.h>
#include "MD_Object.h"
#include "math/TRS_data.h"

MD_Object::MD_Object(MD_Shape* _shape, MD_Material* _material, const TRSDataf& trsData)
    : shape(_shape), material(_material), trs(trsData)
{}

void MD_Object::draw(MD_Shader& shader) {
    if (!shape) return;

    const MD_Mesh* mesh = shape->getMesh();
    glBindVertexArray(mesh->VAO);

    // GPU builds the matrix in the vertex shader from these three uniforms
    shader.setVec3("uTranslation", trs.translation);
    shader.setVec3("uRotation", trs.rotation);
    shader.setVec3("uScale", trs.scale);

    if (material) {
        MD_Texture* texture = dynamic_cast<MD_Texture*>(material);
        if (texture) {
            glActiveTexture(GL_TEXTURE0);
            shader.setInt("texture0", 0);
            glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
        }
    }

    glDrawElements(GL_TRIANGLES,
                   (GLsizei)mesh->data->indices.size(),
                   GL_UNSIGNED_INT, 0);
}