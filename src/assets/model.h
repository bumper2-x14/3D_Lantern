#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "mesh_data.h"


class Model{
    public:
        Model();
        Model(const std::string& path);
        ~Model();
        
        // delete copy
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        // move
        Model(Model&& other) noexcept;
        Model& operator=(Model&& other) noexcept;

        bool loadModel(const std::string& path);
        const MeshData& getMesh() const ;
        MeshData& getMesh();

        bool hasMesh() const { return mesh != nullptr; }
        
        static void regressionTest();

    private:
        MeshData* mesh = nullptr;

        bool loadOBJ(const std::string& path);
       
};

#endif 