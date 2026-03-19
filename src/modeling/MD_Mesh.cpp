#include <iostream>
#include <vector>
#include <glad/glad.h>

#include "assets/mesh_data.h"
#include "assets/vertex.h"
#include "MD_Mesh.h"

MD_Mesh::MD_Mesh(): data(nullptr) {}

MD_Mesh::MD_Mesh(const std::vector<Vertex>& _vertices,
                     const std::vector<unsigned int>& _indice){
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

bool MD_Mesh::setupMD_Mesh(){
    if(data == nullptr)
         return false;

    if(data->indices.empty() || data->vertices.empty()) 
        return false;


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    if (glGetError() != GL_NO_ERROR)
        return false;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
                     data->vertices.size() * sizeof(Vertex),
                     data->vertices.data(),
                      GL_STATIC_DRAW); 

    if (glGetError() != GL_NO_ERROR)
        return false;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     data->indices.size() * sizeof(unsigned int),
                     data->indices.data(), GL_STATIC_DRAW);

    if (glGetError() != GL_NO_ERROR)
        return false;

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                             sizeof(Vertex),
                              (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                             sizeof(Vertex),
                             (void*)offsetof(Vertex, normal));

    if (glGetError() != GL_NO_ERROR)
        return false;  

    glBindVertexArray(0);
    return true;
}

void MD_Mesh::regressionTest(){
    std::vector<Vertex> v;

    v.push_back(Vertex(Vec3f(1,0,0), Vec3f(0,1,0), Vec2f(0,0)));
    v.push_back(Vertex(Vec3f(0,1,0), Vec3f(0,1,0), Vec2f(1,0)));

    std::vector<unsigned int> i = {0,1};

    MD_Mesh mesh(v,i);

    assert(mesh.data != nullptr);
    assert(mesh.data->vertices.size() == 2);
    assert(mesh.data->indices.size() == 2);
    assert(mesh.data->indices[0] == 0);

    bool succes = mesh.setupMD_Mesh();
    assert(succes);

    MeshData original(v,i);
    MD_Mesh mesh2(&original);

    assert(mesh2.data != nullptr);
    assert(mesh2.data->vertices.size() == original.vertices.size());
    assert(mesh2.data->indices.size() == original.indices.size());
    assert(mesh2.data->indices[0] == original.indices[0]);
    //deep copy check
    assert(mesh2.data != &original); 
    std::cout<<"All MD_Mesh tests passed successfully\n";
}
