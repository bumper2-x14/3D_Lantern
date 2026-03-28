#include "MD_Circle.h"

MD_Circle::MD_Circle(int _nb_seg){
    nb_seg = _nb_seg;

    buildShape();
    mesh.setupMD_Mesh();
}

void MD_Circle::buildShape(){
    // Angle between segments
    float step = 2 * M_PI / nb_seg;

    // Asigning normal value
    Vec3f n (0, 1, 0);

    // Build each triangle
    for (int i=0 ; i<nb_seg ; i++){
        
        float alpha = i * step;
        float alpha2 = (i+1) * step;

        // Triangle positions
        Vec3f p0(cos(alpha), 0, sin(alpha));
        Vec3f p1(0, 0,0);
        Vec3f p2(cos(alpha2), 0, sin(alpha2));

        // Texture coordinates
        Vec2f uv0(0.0f, 0.0f);
        Vec2f uv1(0.0f, 0.0f);
        Vec2f uv2(0.0f, 0.0f);

        // First vertex index
        unsigned int start = mesh.data->vertices.size();

        // Add triangle vertices
        mesh.data->vertices.push_back(Vertex(p0,n,uv0));
        mesh.data->vertices.push_back(Vertex(p1,n,uv1));
        mesh.data->vertices.push_back(Vertex(p2,n,uv2));

        // Add triangle indices
        mesh.data->indices.push_back(start + 0); 
        mesh.data->indices.push_back(start + 1);
        mesh.data->indices.push_back(start + 2);


    } 
}

void MD_Circle::applyTransform(Transform* transform){
    // Transform all vertices
     for (auto& v : mesh.data->vertices){
        v.position = transform->mat * v.position;
        v.normal = transform->transformNormal(v.normal);
     }
}