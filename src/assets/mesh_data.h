#ifndef MESHDATA_H
#define MESHDATA_H

#include <vector>

#include "vertex.h"

// Triangle structure represent the indices of the three vertices that form a triangle
struct Triangle {
    int v0, v1, v2;
};


class MeshData {
    public:
        std::vector<Vertex> vertices;
        std::vector<Triangle> tris;

        void computeNormals();
};

#endif