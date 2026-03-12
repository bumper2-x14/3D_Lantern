#ifndef MESHDATA_H
#define MESHDATA_H

#include <vector>

#include "vertex.h"


class MeshData {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        MeshData();
        MeshData(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indice);

};

#endif