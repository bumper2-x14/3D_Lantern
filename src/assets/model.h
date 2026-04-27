#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "mesh_data.h"

/// @brief Represents a 3D model loaded from a file (mainly OBJ).
///        Stores mesh data and handles loading.
class Model{
    public:
        /// @brief Default constructor (empty model).
        Model();

        /// @brief Loads a model directly from a file.
        /// @param path Path to model file.
        Model(const std::string& path);

        /// @brief Destructor (frees mesh if allocated).
        ~Model();
        
        // delete copy

        /// @brief Disable copy (model owns mesh data).
        Model(const Model&) = delete;

        /// @brief Disable copy assignment.
        Model& operator=(const Model&) = delete;

        // move

        /// @brief Move constructor (transfers mesh ownership).
        Model(Model&& other) noexcept;

        /// @brief Move assignment (transfers mesh ownership).
        Model& operator=(Model&& other) noexcept;

        /// @brief Loads a model from file.
        /// @param path Path to file.
        /// @return true if loading succeeded.
        bool loadModel(const std::string& path);

        /// @brief Returns mesh as const reference.
        const MeshData& getMesh() const ;

        /// @brief Returns mesh pointer (modifiable).
        MeshData* getMesh();
        
        /// @brief Returns model name (usually file name).
        const std::string& getName() const { return name; }

        /// @brief Static helper to load OBJ file into MeshData.
        /// @param path Path to OBJ file.
        /// @param success Output flag.
        static MeshData loadOBJ(const std::string& path, bool& success);

        /// @brief Checks if model has valid mesh data.
        bool hasMesh() const { return mesh != nullptr; }
        
        /// @brief Runs basic tests for model loading.
        static void regressionTest();

    private:
        MeshData* mesh = nullptr; ///< Pointer to mesh data.
        std::string name; ///< Model name.

        /// @brief Internal OBJ loader.
        bool loadOBJ(const std::string& path);       
};

#endif