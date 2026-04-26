
#include "MD_Box.h"

MD_Box::MD_Box(float _size){

    size = _size;
    mesh = new MD_Mesh;
    buildShape();
    mesh->setupMD_Mesh();
}

void MD_Box::addFace(Vec3f p0, Vec3f p1, Vec3f p2, Vec3f p3, Vec3f normal){
    unsigned int start = mesh->data->vertices.size();

    mesh->data->vertices.push_back(Vertex(p0, normal, Vec2f(0.0f, 0.0f)));
    mesh->data->vertices.push_back(Vertex(p1, normal, Vec2f(1.0f, 0.0f)));
    mesh->data->vertices.push_back(Vertex(p2, normal, Vec2f(0.0f, 1.0f)));
    mesh->data->vertices.push_back(Vertex(p3, normal, Vec2f(1.0f, 1.0f)));

    mesh->data->indices.push_back(start + 0);
    mesh->data->indices.push_back(start + 1);
    mesh->data->indices.push_back(start + 2);

    mesh->data->indices.push_back(start + 2);
    mesh->data->indices.push_back(start + 1);
    mesh->data->indices.push_back(start + 3);
}

void MD_Box::buildShape(){
    float h = size / 2.0f;

    // Top
    addFace(
        Vec3f(-h,  h, -h), Vec3f( h,  h, -h),
        Vec3f(-h,  h,  h), Vec3f( h,  h,  h),
        Vec3f(0, 1, 0)
    );

    // Bottom
    addFace(
        Vec3f(-h, -h,  h), Vec3f( h, -h,  h),
        Vec3f(-h, -h, -h), Vec3f( h, -h, -h),
        Vec3f(0, -1, 0)
    );

    // Front
    addFace(
        Vec3f(-h, -h,  h), Vec3f( h, -h,  h),
        Vec3f(-h,  h,  h), Vec3f( h,  h,  h),
        Vec3f(0, 0, 1)
    );

    // Back
    addFace(
        Vec3f( h, -h, -h), Vec3f(-h, -h, -h),
        Vec3f( h,  h, -h), Vec3f(-h,  h, -h),
        Vec3f(0, 0, -1)
    );

    // Right
    addFace(
        Vec3f( h, -h,  h), Vec3f( h, -h, -h),
        Vec3f( h,  h,  h), Vec3f( h,  h, -h),
        Vec3f(1, 0, 0)
    );

    // Left
    addFace(
        Vec3f(-h, -h, -h), Vec3f(-h, -h,  h),
        Vec3f(-h,  h, -h), Vec3f(-h,  h,  h),
        Vec3f(-1, 0, 0)
    );
}

void MD_Box::applyTransform(Transform* transform){
    for (auto& v : mesh->data->vertices){
        v.position = transform->mat * v.position;
        v.normal = transform->transformNormal(v.normal);
    }
}