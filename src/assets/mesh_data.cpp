#include <vector>

#include "vertex.h"
#include "mesh_data.h"

MeshData::MeshData(){}

MeshData::MeshData(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indice): 
    vertices(_vertices), indices(_indice) {}

