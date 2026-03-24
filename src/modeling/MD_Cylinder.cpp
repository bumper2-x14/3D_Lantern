#include "MD_Cylinder.h"

MD_Cylinder::MD_Cylinder(int _nb_seg){
    nb_seg = _nb_seg;

    buildShape();
    mesh.setupMD_Mesh();
}

void MD_Cylinder::buildShape(){
    float step =( 2 * M_PI ) / nb_seg; 

    for(int i = 0 ; i < nb_seg ; i++){
        float alpha = i * step;
        float alpha2 = (i+1) * step; 

        // Asigning position
        Vec3f p0( cos(alpha), 1, sin(alpha) );
        Vec3f p1( cos(alpha), -1, sin(alpha) );
        Vec3f p2( cos(alpha2), 1, sin(alpha2) );
        Vec3f p3( cos(alpha2), -1, sin(alpha2) );

        // Asigning normal value
        Vec3f n0( cos(alpha), 0, sin(alpha) );
        Vec3f n1( cos(alpha), 0, sin(alpha) );
        Vec3f n2( cos(alpha2), 0, sin(alpha2) );
        Vec3f n3( cos(alpha2), 0, sin(alpha2) );

        // Asigning texteur
        Vec2f uv0(0.0f, 0.0f);
        Vec2f uv1(0.0f, 0.0f);
        Vec2f uv2(0.0f, 0.0f);
        Vec2f uv3(0.0f, 0.0f);

        unsigned int start = mesh.data->vertices.size();

        mesh.data->vertices.push_back(Vertex(p0, n0, uv0));   
        mesh.data->vertices.push_back(Vertex(p1, n1, uv1));   
        mesh.data->vertices.push_back(Vertex(p2, n2, uv2));   
        mesh.data->vertices.push_back(Vertex(p3, n3, uv3));

        mesh.data->indices.push_back(start + 0); 
        mesh.data->indices.push_back(start + 1);
        mesh.data->indices.push_back(start + 2);

        mesh.data->indices.push_back(start + 2);
        mesh.data->indices.push_back(start + 1);
        mesh.data->indices.push_back(start + 3);

    }
}

 void MD_Cylinder::applyTransform(Transform* transform) {
    for (auto& v : mesh.data->vertices){
            v.position = transform->mat * v.position;
            v.normal = transform->transformNormal(v.normal);
        }
 } 
