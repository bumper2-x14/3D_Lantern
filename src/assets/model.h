#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "mesh_data.h"


class Model{
    public:
        Model();
        Model(const std::string& path);
        bool loadModel(const std::string& path);
        const MeshData& getMesh() const ;
        MeshData& getMesh();
        static void regressionTest();

    private:
        MeshData mesh;

        bool loadOBJ(const std::string& path);
       
};

#endif 