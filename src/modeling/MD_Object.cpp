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

    // MATERIAL BIND
    if (material) {
        material->bind(shader);
    }

    // DRAW
    glDrawElements(GL_TRIANGLES,
                   (GLsizei)mesh->data->indices.size(),
                   GL_UNSIGNED_INT, 0);

    // MATERIAL UNBIND
    if (material) {
        material->unbind();
    }

    glBindVertexArray(0);
}
