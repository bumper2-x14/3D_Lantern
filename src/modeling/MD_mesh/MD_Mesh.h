#ifndef MD_MESH_H
#define MD_MESH_H

#include <vector>

#include "../../assets/mesh_data.h"

class MD_Mesh {
    public:
        MeshData* data;
        void setupMD_Mesh();

    private:      
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
};

#endif