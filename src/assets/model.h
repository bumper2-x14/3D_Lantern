#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "mesh_data.h"


class Model{
    public:
    
        Model(const std::string& path);
        bool loadModel(const std::string& path);
        const std::vector<MeshData>& getMeshes() const ;
        static void regressionTest();

    private:
        std::vector<MeshData> meshes;

        bool loadOBJ(const std::string& path, MeshData& outMeshes);
       
};

#endif 