#include <glad/glad.h>
#include "MD_Object.h"
/*
MD_Object::MD_Object(): shape(nullptr), transform(nullptr), material(nullptr) {}
*/
MD_Object::MD_Object(MD_Shape* _shape, Transform* _transform,
        MD_Material* _material): shape (_shape), transform(_transform), material(_material) {
    if (transform != nullptr) shape->applyTransform(transform);
}

void MD_Object::draw(MD_Shader& shader){
   
    const MD_Mesh& mesh = shape->getMesh();
    glBindVertexArray(mesh.VAO);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonOffset(1.0f, 1.0f);
       
    shader.setMat4("localToWorld" , transform->mat);
    
    //glUniform1i(glGetUniformLocation(shaderID, "uWireframe"), 0);
    //shader.setBool("uWireframe",0);
    //glDrawElements(GL_TRIANGLES, (GLsizei)mesh.data->indices.size(),     GL_UNSIGNED_INT, 0);
    //glDisable(GL_POLYGON_OFFSET_FILL);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //glUniform1i(glGetUniformLocation(shaderID, "uWireframe"), 1);
    //shader.setBool("uWireframe" , 1);

    
    if(material!=nullptr){
        //glActiveTexture(GL_TEXTURE0);
        MD_Texture* texture=dynamic_cast<MD_Texture*>(material);
        if(texture!=nullptr){
            glActiveTexture(GL_TEXTURE0);
            shader.setInt("texture0", 0);
            glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
        }
    }
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.data->indices.size(), GL_UNSIGNED_INT, 0);
} 
    //glDrawArrays(GL_TRIANGLES, 0, mesh.data->vertices.size()); 
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
    /*
     const MD_Mesh& mesh = shape->getMesh();
    
    glBindVertexArray(mesh.VAO);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
   
    shader.setMat4("localToWorld" , transform->mat);
    //glUniform1i(glGetUniformLocation(shaderID, "uWireframe"), 0);
    shader.setBool("uWireframe",0);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.data->indices.size(), GL_UNSIGNED_INT, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
    //glUniform1i(glGetUniformLocation(shaderID, "uWireframe"), 1);
    shader.setBool("uWireframe" , 1);
   
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.data->indices.size(), GL_UNSIGNED_INT, 0);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    */
