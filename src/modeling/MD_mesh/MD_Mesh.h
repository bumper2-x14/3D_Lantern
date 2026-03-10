#ifndef MD_MESH_H
#define MD_MESH_H

#include <vector>

#include "../../assets/mesh_data.h"

class MD_Mesh {
    public:
        MeshData* data;
        MD_Mesh(const MeshData* _data);
        MD_Mesh(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indice);

        void setupMD_Mesh();


    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
};

#endif