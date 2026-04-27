#include "MD_Disk.h"

MD_Disk::MD_Disk(int _nb_seg){
    nb_seg = _nb_seg;
    type = ShapeType::DISK;
    mesh = new MD_Mesh;
    buildShape();
    mesh->setupMD_Mesh();
}


void MD_Disk::buildShape(){
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
        Vec2f uv0(0.5 * cos( alpha)+0.5,0.5 * sin(alpha )+0.5);
        Vec2f uv1(0.5f, 0.5f);
        Vec2f uv2(0.5 * cos( alpha2)+0.5,0.5 * sin(alpha2)+0.5);

        // First vertex index
        unsigned int start = mesh->data->vertices.size();

        // Add triangle vertices
        mesh->data->vertices.push_back(Vertex(p0,n,uv0));
        mesh->data->vertices.push_back(Vertex(p1,n,uv1));
        mesh->data->vertices.push_back(Vertex(p2,n,uv2));

        // Add triangle indices
        mesh->data->indices.push_back(start + 0); 
        mesh->data->indices.push_back(start + 1);
        mesh->data->indices.push_back(start + 2);


    } 
}

void MD_Disk::applyTransform(Transform* transform){
    // Transform all vertices
     for (auto& v : mesh->data->vertices){
        v.position = transform->mat * v.position;
        v.normal = transform->transformNormal(v.normal);
     }
}