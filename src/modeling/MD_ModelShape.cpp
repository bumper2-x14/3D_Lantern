#include "MD_ModelShape.h"

MD_ModelShape::MD_ModelShape(const std::string& _path) : path(_path){
    type = ShapeType::MESH;

    mesh = new MD_Mesh;
    buildShape();
    mesh->setupMD_Mesh();
}

void MD_ModelShape::buildShape() {
   
}

