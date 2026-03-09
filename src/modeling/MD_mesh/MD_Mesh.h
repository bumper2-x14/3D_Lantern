#ifndef MD_MESH_H
#define MD_MESH_H

#include <vector>

#include "../../assets/mesh_data.h"

class MD_Mesh {
    public:
    
    private:
        MeshData* data;
    
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
};

#endif