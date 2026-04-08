#ifndef RT_MESH_H
#define RT_MESH_H

#include "RT_Object.h"
#include "RT_Material.h"
#include "RT_Triangle.h"
#include "RT_BVH.h"
#include "assets/mesh_data.h"
#include <vector>

class RT_Mesh : public RT_Object {
public:
    RT_Mesh(const MeshData& meshData, RT_Material* material);
    ~RT_Mesh();
    void setTransform(const TRSTransformd& t) override;
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

private:
    std::vector<RT_Triangle> triangles;
    RT_Material* material;
    BVHNode* bvh = nullptr;
};

#endif