#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include "mesh_data.h"

class Model{
    public:
    
        Model(const std::string& path);

    private:
        std::vector<MeshData> meshes;

        void loadModel(std::string path);
        MeshData loadOBJ(std::string path);
       
};

#endif 