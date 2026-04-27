#ifndef MESHDATA_H
#define MESHDATA_H

#include <vector>

#include "vertex.h"

/// @brief Stores raw mesh data (vertices + indices).
///        Used before sending data to GPU (MD_Mesh).
class MeshData {
    public:
        std::vector<Vertex> vertices; ///< List of vertices.
        std::vector<unsigned int> indices; ///< Index buffer.

        /// @brief Default constructor (empty mesh).
        MeshData();

        /// @brief Creates mesh data from given vertices and indices.
        /// @param _vertices List of vertices.
        /// @param _indice List of indices.
        MeshData(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indice);

        /// @brief Runs basic tests to validate mesh data.
        static void regressionTest();
};

#endif