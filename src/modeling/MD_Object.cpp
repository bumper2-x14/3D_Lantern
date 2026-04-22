#include <glad/glad.h>
#include "MD_Object.h"
#include "math/TRS_data.h"

MD_Object::MD_Object(const std::string& _name, MD_Shape* _shape, MD_Material* _material, const TRSDataf& _trsData)
    : name(_name), shape(_shape), material(_material), trs(_trsData)
{}

Mat4f MD_Object::getTransformMatrix() const {
    return Mat4f::TRS(trs.translation, trs.rotation, trs.scale);
}

std::string MD_Object::serialize() const {
    std::string result = "object {\n";

    result += "    name " + name + "\n";
    result += "    type " + shape->typeToString(shape->type) + "\n";

    result += "    material_ref default_mat\n";

    result += "    translate "
           + std::to_string(trs.translation.x) + " "
           + std::to_string(trs.translation.y) + " "
           + std::to_string(trs.translation.z) + "\n";

    result += "    rotate "
           + std::to_string(trs.rotation.x) + " "
           + std::to_string(trs.rotation.y) + " "
           + std::to_string(trs.rotation.z) + "\n";

    result += "    scale "
           + std::to_string(trs.scale.x) + " "
           + std::to_string(trs.scale.y) + " "
           + std::to_string(trs.scale.z) + "\n";

    result += "}\n";

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

