#include <vector>
#include <cmath>

#include "MD_Sphere.h"

MD_Sphere::MD_Sphere(float _longtitude_seg , float _latitude_seg ) : 
        latitude_seg ( _latitude_seg), longitude_seg(_longtitude_seg )  {

    type = ShapeType::SPHERE;

    mesh = new MD_Mesh;
    buildShape();
    mesh->setupMD_Mesh();
}


void MD_Sphere::buildShape(){
    
    float delta1 = M_PI /(2.0 * latitude_seg );
    float delta2 = (2 * M_PI) / longitude_seg;

    for (float alpha = -M_PI /2 ; alpha < M_PI/2.0 ; alpha += delta1){
        float alpha2 = alpha + delta1;

        for (float beta=0 ; beta < 2.0 * M_PI ; beta += delta2){
            float beta2 = beta + delta2;
           
            Vec3f p0(cos(alpha2) * cos(beta),  sin(alpha2),  cos(alpha2) * sin(beta));
            Vec3f p1(cos(alpha)  * cos(beta),  sin(alpha),   cos(alpha)  * sin(beta));
            Vec3f p2(cos(alpha2) * cos(beta2), sin(alpha2),  cos(alpha2) * sin(beta2));
            Vec3f p3(cos(alpha)  * cos(beta2), sin(alpha),   cos(alpha)  * sin(beta2));

            // Asigning normal values
            Vec3f n0 = p0;
            Vec3f n1 = p1;
            Vec3f n2 = p2;
            Vec3f n3 = p3;

            float ux0 = (float)beta / (2.0 * M_PI);
            float ux1 = (float)beta2 / (2.0 * M_PI);

            float vx0 = (float)(M_PI/2 + alpha ) / M_PI;
            float vx1 = (float)(M_PI/2 + alpha2) / M_PI;

            // Asigning texteur
            Vec2f uv0(ux0, vx1);
            Vec2f uv1(ux0, vx0);
            Vec2f uv2(ux1, vx1);
            Vec2f uv3(ux1, vx0);

            unsigned int start = mesh->data->vertices.size();

            mesh->data->vertices.push_back(Vertex(p0, n0, uv0));   
            mesh->data->vertices.push_back(Vertex(p1, n1, uv1));   
            mesh->data->vertices.push_back(Vertex(p2, n2, uv2));   
            mesh->data->vertices.push_back(Vertex(p3, n3, uv3));   

            mesh->data->indices.push_back(start + 0); 
            mesh->data->indices.push_back(start + 2);
            mesh->data->indices.push_back(start + 1);

            mesh->data->indices.push_back(start + 1);
            mesh->data->indices.push_back(start + 2);
            mesh->data->indices.push_back(start + 3);

        }
    }
}

void MD_Sphere::applyTransform(Transform* transform){
    //for (int i=0 ; i < mesh->data->vertices.size() ; i++)
    for (auto& v : mesh->data->vertices){
        v.position = transform->mat * v.position;
        v.normal = transform->transformNormal(v.normal);
    }
}

void MD_Sphere::regressionTest(){
    MD_Sphere sphereTest(10, 10);

    const MD_Mesh* mesh = sphereTest.getMesh();

    assert(mesh != nullptr && "Mesh is null");
    assert(mesh->data != nullptr && "Mesh data is null");

    assert(!mesh->data->indices.empty() && "Indices list is empty");
    assert(!mesh->data->vertices.empty() && "Vertex list is empty");
    
    const Vertex vInitial = sphereTest.mesh->data->vertices[0];
    float len = std::sqrt(
                    vInitial.position.x * vInitial.position.x +
                    vInitial.position.y * vInitial.position.y +
                    vInitial.position.z * vInitial.position.z);
    
    assert((len - 1.0) < 0.001f && "First vertex is not on the unit sphere");  

    Transform tr = Transform::translate(Vec3f(1.0f, 0.0f, 0.0f));
    sphereTest.applyTransform(&tr);
    Vertex vAfter = sphereTest.mesh->data->vertices[0];
    assert(std::abs(vAfter.position.x - (vInitial.position.x + 1.0f)) < 0.001f &&
       "Translation on X axis failed");

    std::cout << "MD_Sphere regression test passed\n";   

}

