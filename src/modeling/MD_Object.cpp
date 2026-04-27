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
    std::string mat_ref = material ? material->getName() : "default_mat";

    // Convert from MD (OpenGL Y-up, +Z forward) to RT (Y-up, Z-depth)
    // RT.x =  MD.x
    // RT.y =  MD.z
    // RT.z = -MD.y
    Vec3f rt_translation = { trs.translation.x,  trs.translation.z, -trs.translation.y };
    Vec3f rt_rotation    = { trs.rotation.x,      trs.rotation.z,   -trs.rotation.y    };
    Vec3f rt_scale       = { trs.scale.x,         trs.scale.z,       trs.scale.y       };

    std::string result = "object {\n";
    result += "    name "         + name + "\n";
    result += "    type "         + shape->typeToString(shape->type) + "\n";
    result += "    material_ref " + mat_ref + "\n";
    result += "    translate "
        + std::to_string(rt_translation.x) + " "
        + std::to_string(rt_translation.y) + " "
        + std::to_string(rt_translation.z) + "\n";
    result += "    rotate "
        + std::to_string(rt_rotation.x) + " "
        + std::to_string(rt_rotation.y) + " "
        + std::to_string(rt_rotation.z) + "\n";
    result += "    scale "
        + std::to_string(rt_scale.x) + " "
        + std::to_string(rt_scale.y) + " "
        + std::to_string(rt_scale.z) + "\n";
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

