#ifndef MD_MESH_H
#define MD_MESH_H

#include <vector>

#include "assets/mesh_data.h"

class MD_Mesh{
    public:
        MeshData* data;
        bool owns_data = true;
        
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        
        MD_Mesh();
        void setData(const MeshData& newData);
        MD_Mesh(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indice);
        MD_Mesh(MeshData* _data);

        ~MD_Mesh();

        bool setupMD_Mesh();    
        static void regressionTest();

};
#endif