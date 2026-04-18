#include "MD_Torus.h"

MD_Torus::MD_Torus(int _nb_ring, int _nb_side){
    nb_ring = _nb_ring;
    nb_side = _nb_side;

    mesh = new MD_Mesh;
    buildShape();
    mesh->setupMD_Mesh();
}

std::string MD_Torus::serialize() const {
    return "torus " + std::to_string(nb_ring) + " " + std::to_string(nb_side);
}

void MD_Torus::buildShape(){
    
    float R = 1.0f;   
    float r = 0.3f;   

    float step_ring = (2.0f * M_PI) / nb_ring;
    float step_side = (2.0f * M_PI) / nb_side;

    for(int i = 0; i < nb_ring; i++){
        float u = i * step_ring;
        float u2 = (i + 1) * step_ring;

        for(int j = 0; j < nb_side; j++){
            float v = j * step_side;
            float v2 = (j + 1) * step_side;

            // points
            Vec3f p0(
                (R + r * cos(v)) * cos(u),
                r * sin(v),
                (R + r * cos(v)) * sin(u)
            );

            Vec3f p1(
                (R + r * cos(v)) * cos(u2),
                r * sin(v),
                (R + r * cos(v)) * sin(u2)
            );

            Vec3f p2(
                (R + r * cos(v2)) * cos(u),
                r * sin(v2),
                (R + r * cos(v2)) * sin(u)
            );

            Vec3f p3(
                (R + r * cos(v2)) * cos(u2),
                r * sin(v2),
                (R + r * cos(v2)) * sin(u2)
            );

            // center of tube for each ring angle
            Vec3f c0(
                R * cos(u),
                0,
                R * sin(u)
            );

            Vec3f c1(
                R * cos(u2),
                0,
                R * sin(u2)
            );

            // normals
            Vec3f n0 = normalize(p0 - c0);
            Vec3f n1 = normalize(p1 - c1);
            Vec3f n2 = normalize(p2 - c0);
            Vec3f n3 = normalize(p3 - c1);

            // texture coordinates
            float ux0 = (float)i / nb_ring;
            float ux1 = (float)(i + 1) / nb_ring;
            float vx0 = (float)j / nb_side;
            float vx1 = (float)(j + 1) / nb_side;

            Vec2f uv0(ux0, vx0);
            Vec2f uv1(ux1, vx0);
            Vec2f uv2(ux0, vx1);
            Vec2f uv3(ux1, vx1);

            unsigned int start = mesh->data->vertices.size();

            mesh->data->vertices.push_back(Vertex(p0, n0, uv0));
            mesh->data->vertices.push_back(Vertex(p1, n1, uv1));
            mesh->data->vertices.push_back(Vertex(p2, n2, uv2));
            mesh->data->vertices.push_back(Vertex(p3, n3, uv3));

            mesh->data->indices.push_back(start + 0);
            mesh->data->indices.push_back(start + 1);
            mesh->data->indices.push_back(start + 2);

            mesh->data->indices.push_back(start + 2);
            mesh->data->indices.push_back(start + 1);
            mesh->data->indices.push_back(start + 3);
        }
    }
}

void MD_Torus::applyTransform(Transform* transform){
    for(auto& v : mesh->data->vertices){
        v.position = transform->mat * v.position;
        v.normal = transform->transformNormal(v.normal);
    }
}