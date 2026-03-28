#include "MD_Quad.h"


MD_Quad::MD_Quad(float _width, float _lenght){
    width = _width;
    length = _lenght;

    buildShape();
    mesh.setupMD_Mesh();
}

void MD_Quad::buildShape(){
    //width & length 
    float hw = width / 2.0f;
    float hl = length / 2.0f;

    // Asigning position
    Vec3f p0(-hw, 0, -hl);
    Vec3f p1(-hw, 0,  hl);
    Vec3f p2( hw, 0, -hl);
    Vec3f p3( hw, 0,  hl);

    // Asigning normal value
    Vec3f n (0, 1, 0);

    // Texture coordinates
    Vec2f uv0(0.0f, 0.0f);
    Vec2f uv1(0.0f, 0.0f);
    Vec2f uv2(0.0f, 0.0f);
    Vec2f uv3(0.0f, 0.0f);

    mesh.data->vertices.push_back(Vertex(p0, n, uv0));   
    mesh.data->vertices.push_back(Vertex(p1, n, uv1));   
    mesh.data->vertices.push_back(Vertex(p2, n, uv2));   
    mesh.data->vertices.push_back(Vertex(p3, n, uv3));

    mesh.data->indices.push_back(0); 
    mesh.data->indices.push_back(1);
    mesh.data->indices.push_back(2);

    mesh.data->indices.push_back(2);
    mesh.data->indices.push_back(1);
    mesh.data->indices.push_back(3);

}

void MD_Quad::applyTransform(Transform* transform){
     for (auto& v : mesh.data->vertices){
        v.position = transform->mat * v.position;
        v.normal = transform->transformNormal(v.normal);
     }
}