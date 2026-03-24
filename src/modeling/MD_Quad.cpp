#include "MD_Quad.h"


MD_Quad::MD_Quad(float _width =1 , float _lenght = 1){
    width = _width;
    length = _lenght;

    buildShape();
    mesh.setupMD_Mesh();
}

void MD_Quad::buildShape(){
    // Asigning position
    Vec3f p0 (-1, 0, -1);
    Vec3f p1 (-1, 0,  1);
    Vec3f p2 ( 1, 0, -1);
    Vec3f p3 ( 1, 0,  1);

    // Asigning normal value
    Vec3f n (0, 1, 0);

    // Asigning texteur
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
    mesh.data
}