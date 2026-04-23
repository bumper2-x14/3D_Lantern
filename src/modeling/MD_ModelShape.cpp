#include "MD_ModelShape.h"

MD_ModelShape::MD_ModelShape(MeshData* _mesh_data) : meshData(_mesh_data) {
    type = ShapeType::MESH;
    mesh = new MD_Mesh(meshData);
    mesh->setupMD_Mesh();
}

void MD_ModelShape::buildShape() {
    // Nothing
}

void MD_ModelShape::applyTransform(Transform* transform) {
    for (auto& v : mesh->data->vertices) {
        v.position = transform->mat * v.position;
        v.normal = transform->transformNormal(v.normal);
    }
}