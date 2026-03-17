#include <glad/glad.h>

#include "MD_Object.h"

MD_Object::MD_Object(): shape(nullptr), transform(nullptr) {}

MD_Object::MD_Object(MD_Shape* _shape, Transform* _transform = nullptr): shape (_shape), transform(_transform) {
    if (transform != nullptr) shape->applyTransform(transform);
}

void MD_Object::draw(){
     const MD_Mesh& mesh = shape->getMesh();

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.data->indices.size(), GL_UNSIGNED_INT, 0);
}