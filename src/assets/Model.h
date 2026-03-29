#ifndef MD_MODEL_H
#define MD_MODEL_H

#include <string>
#include <vector>

#include "mesh_data.h"

class MD_Model{
    public:
    
        MD_Model(const std::string& path);
        void draw(MD_Shader& shader);

    private:
        std::vector<MeshData> meshes;

        void loadModel(std::string path);
        MD_Mesh loadOBJ(std::string path);
       
};

#endif 