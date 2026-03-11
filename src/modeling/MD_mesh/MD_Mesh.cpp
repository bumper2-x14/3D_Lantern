#include <vector>

#include "../../assets/mesh_data.h"
#include "../../assets/vertex.h"
#include "MD_Mesh.h"

MD_Mesh::MD_Mesh(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indice){
    data = new MeshData;
    data->vertices = _vertices;
    data->indices = _indice;
}

MD_Mesh::MD_Mesh(const MeshData* _data){
    data = new MeshData(*_data);
}
MD_Mesh::~MD_Mesh(){
    delete data;
}

void MD_Mesh::setupMD_Mesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(Vertex), data->vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indices.size() * sizeof(unsigned int), data->indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}