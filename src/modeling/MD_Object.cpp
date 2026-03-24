#include <glad/glad.h>
#include "MD_Object.h"

MD_Object::MD_Object(): shape(nullptr), transform(nullptr) {}

MD_Object::MD_Object(MD_Shape* _shape, Transform* _transform = nullptr): shape (_shape), transform(_transform) {
    if (transform != nullptr) shape->applyTransform(transform);
}


void MD_Object::draw( MD_Shader& shader){
    const MD_Mesh& mesh = shape->getMesh();
    
    glBindVertexArray(mesh.VAO);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
   
    //glUniform1i(glGetUniformLocation(shaderID, "uWireframe"), 0);
    shader.setBool("uWireframe",0);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.data->indices.size(), GL_UNSIGNED_INT, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
    //glUniform1i(glGetUniformLocation(shaderID, "uWireframe"), 1);
    shader.setBool("uWireframe" , 1);
   
    shader.setMat4("localToWorld" , transform->mat);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.data->indices.size(), GL_UNSIGNED_INT, 0);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
