#include "MD_Cone.h"

MD_Cone::MD_Cone(int _nb_seg){
    nb_seg = _nb_seg;

    mesh = new MD_Mesh;
    buildShape();
    mesh->setupMD_Mesh();
}

void MD_Cone::buildShape(){
    float step = 2 * M_PI / nb_seg;
    
    
    for(int i = 0; i < nb_seg; i++){
        float alpha  = i * step;
        float alpha2 = (i + 1) * step;

        // positions
        Vec3f p0( std::cos(alpha),  0, std::sin(alpha) );   // base point 1
        Vec3f p1( 0, 1, 0 );                                // apex
        Vec3f p2( std::cos(alpha2), 0, std::sin(alpha2) ); // base point 2

        // normals
        Vec3f n0( std::cos(alpha) / std::sqrt(2.0f),
                     1.0f / std::sqrt(2.0f),
                      std::sin(alpha)  / std::sqrt(2.0f) );

        Vec3f n1( std::cos(alpha) / std::sqrt(2.0f),
                     1.0f / std::sqrt(2.0f),
                      std::sin(alpha)  / std::sqrt(2.0f) );

        Vec3f n2( std::cos(alpha2) / std::sqrt(2.0f),
                     1.0f / std::sqrt(2.0f),
                     std::sin(alpha2) / std::sqrt(2.0f) );

        // texture coordinates
        Vec2f uv0( (float)i / nb_seg, 0.0f );
        Vec2f uv1( 0.5f, 1.0f );
        Vec2f uv2( (float)(i + 1) / nb_seg, 0.0f );

        unsigned int start = mesh->data->vertices.size();

        mesh->data->vertices.push_back(Vertex(p0, n0, uv0));
        mesh->data->vertices.push_back(Vertex(p1, n1, uv1));
        mesh->data->vertices.push_back(Vertex(p2, n2, uv2));

        mesh->data->indices.push_back(start + 0);
        mesh->data->indices.push_back(start + 1);
        mesh->data->indices.push_back(start + 2);
    }
}

void MD_Cone::applyTransform(Transform* transform){
    for(auto& v : mesh->data->vertices){
        v.position = transform->mat * v.position;
        v.normal = transform->transformNormal(v.normal);
    }
}

