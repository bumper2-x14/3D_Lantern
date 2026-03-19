#include <vector>
#include <iostream>

#include "vertex.h"
#include "mesh_data.h"

MeshData::MeshData(){}

MeshData::MeshData(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indice): 
    vertices(_vertices), indices(_indice) {}

void MeshData::regressionTest(){
    std::vector<Vertex> v;

    v.push_back(Vertex(Vec3f(1,0,0), Vec3f(0,1,0), Vec2f(0,0)));
    v.push_back(Vertex(Vec3f(0,1,0), Vec3f(0,1,0), Vec2f(1,0)));

    std::vector<unsigned int> i = {0,1};

    MeshData m(v,i);

    assert(m.vertices.size() == 2);
    assert(m.indices.size() == 2);

    assert(m.indices[0] == 0);
    assert(m.indices[1] == 1);

    std::cout<<"All mesh_data tests passed successfully\n";
}