#include <glad/glad.h>
#include "MD_Object.h"
#include "math/TRS_data.h"

MD_Object::MD_Object(MD_Shape* _shape, MD_Material* _material, const TRSDataf& trsData)
    : shape(_shape), material(_material), trs(trsData)
{}

Mat4f MD_Object::getTransformMatrix() const {
    return Mat4f::TRS(trs.translation, trs.rotation, trs.scale);
}

std::string MD_Object::serialize() const {
    std::string result = "object ";

    if (shape) {
        result += shape->serialize();
    } else {
        result += "null_shape";
    }

    result += " trs ";
    result += std::to_string(trs.translation.x) + " ";
    result += std::to_string(trs.translation.y) + " ";
    result += std::to_string(trs.translation.z) + " ";

    result += std::to_string(trs.rotation.x) + " ";
    result += std::to_string(trs.rotation.y) + " ";
    result += std::to_string(trs.rotation.z) + " ";

    result += std::to_string(trs.scale.x) + " ";
    result += std::to_string(trs.scale.y) + " ";
    result += std::to_string(trs.scale.z);

    return result;
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
