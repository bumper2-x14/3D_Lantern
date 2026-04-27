#ifndef RT_MESH_H
#define RT_MESH_H

#include "RT_Object.h"
#include "RT_Material.h"
#include "RT_Triangle.h"
#include "RT_BVH.h"
#include "assets/mesh_data.h"
#include <vector>

/// @brief Ray tracing mesh made of triangles.
///        Uses a BVH to speed up intersection tests.
class RT_Mesh : public RT_Object {
public:
    /// @brief Builds a mesh from mesh data and a material.
    /// @param meshData Source mesh (vertices + indices).
    /// @param material Material used for shading.
    RT_Mesh(const MeshData& meshData, RT_Material* material);

    /// @brief Destructor (cleans BVH if needed).
    ~RT_Mesh();

    /// @brief Applies a transform (position, rotation, scale).
    void setTransform(const TRSTransformd& t) override;

    /// @brief Checks intersection between ray and mesh.
    ///        Uses BVH for efficiency.
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

private:
    std::vector<RT_Triangle> triangles; ///< List of triangles composing the mesh.
    RT_Material* material; ///< Material used for rendering.
    BVHNode* bvh = nullptr; ///< Acceleration structure.
};

#endif