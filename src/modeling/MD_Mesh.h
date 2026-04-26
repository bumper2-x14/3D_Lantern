#ifndef MD_MESH_H
#define MD_MESH_H

#include <vector>

#include "assets/mesh_data.h"

/// @brief Represents a mesh ready to be sent to OpenGL (VAO/VBO/EBO).
///        Wraps MeshData and handles GPU buffers.
class MD_Mesh{
    public:
        MeshData* data; ///< Pointer to mesh data (vertices + indices).
        bool owns_data = true; ///< Indicates if this class is responsible for deleting data.
        
        unsigned int VAO; ///< OpenGL Vertex Array Object.
        unsigned int VBO; ///< OpenGL Vertex Buffer Object.
        unsigned int EBO; ///< OpenGL Element Buffer Object.
        
        /// @brief Default constructor (empty mesh).
        MD_Mesh();

        /// @brief Replaces current mesh data with a copy of newData.
        /// @param newData Mesh data to copy.
        void setData(const MeshData& newData);

        /// @brief Creates a mesh from given vertices and indices.
        /// @param _vertices List of vertices.
        /// @param _indice List of indices.
        MD_Mesh(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indice);

        /// @brief Creates a mesh from an existing MeshData pointer.
        /// @param _data Pointer to mesh data.
        MD_Mesh(MeshData* _data);

        /// @brief Destructor (frees mesh data if owned and GPU buffers).
        ~MD_Mesh();

        /// @brief Initializes OpenGL buffers (VAO, VBO, EBO) and uploads data to GPU.
        /// @return true if setup succeeded, false otherwise.
        bool setupMD_Mesh();    

        /// @brief Runs basic tests to validate mesh creation and setup.
        static void regressionTest();

};

#endif