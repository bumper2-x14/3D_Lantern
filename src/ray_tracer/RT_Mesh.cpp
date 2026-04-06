#include "RT_Mesh.h"

RT_Mesh::RT_Mesh(const MeshData& _mesh_data, RT_Material* _material)
    : material(_material) {
    triangles.reserve(_mesh_data.indices.size() /3);
    for (size_t i = 0; i + 2 < _mesh_data.indices.size(); i += 3) {
        const Vertex& a = _mesh_data.vertices[_mesh_data.indices[i + 0]];
        const Vertex& b = _mesh_data.vertices[_mesh_data.indices[i + 1]];
        const Vertex& c = _mesh_data.vertices[_mesh_data.indices[i + 2]];
        triangles.emplace_back(
            Vec3d(a.position.x, a.position.y, a.position.z),
            Vec3d(b.position.x, b.position.y, b.position.z),
            Vec3d(c.position.x, c.position.y, c.position.z),
            Vec3d(a.normal.x,   a.normal.y,   a.normal.z),
            Vec3d(b.normal.x,   b.normal.y,   b.normal.z),
            Vec3d(c.normal.x,   c.normal.y,   c.normal.z),
            Vec2d(a.uv.x, a.uv.y),
            Vec2d(b.uv.x, b.uv.y),
            Vec2d(c.uv.x, c.uv.y),
            material
        );
    }
}

RT_Mesh::~RT_Mesh() {
    delete bvh;
}

void RT_Mesh::setTransform(const TRSTransformd& t) {
    if (triangles.empty()) return;
    RT_Object::setTransform(t);

    for (auto& tri : triangles)
        tri.setTransform(t);

    // build pointer list for BVH
    RT_ObjectList list;
    for (auto& tri : triangles)
        list.add(&tri);

    delete bvh;
    bvh = new BVHNode(list);
    std::cerr << "Root BVH bbox: x[" << bvh->getBoundingBox().x.min 
          << ", " << bvh->getBoundingBox().x.max << "]" << std::endl;
setBoundingBox(bvh->getBoundingBox());
std::cerr << "Mesh bbox: x[" << getBoundingBox().x.min 
          << ", " << getBoundingBox().x.max << "]" << std::endl;

          std::cerr << "Root BVH bbox: "
          << "x[" << bvh->getBoundingBox().x.min << ", " << bvh->getBoundingBox().x.max << "] "
          << "y[" << bvh->getBoundingBox().y.min << ", " << bvh->getBoundingBox().y.max << "] "
          << "z[" << bvh->getBoundingBox().z.min << ", " << bvh->getBoundingBox().z.max << "]"
          << std::endl;
     
    setBoundingBox(bvh->getBoundingBox());
}

bool RT_Mesh::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    if (!bvh) return false;
    return bvh->rayIntersect(ray, t_interval, rec);
}

/*
void RT_Mesh::setTransform(const TRSTransformd& t) {
    if (triangles.empty()) return;
    RT_Object::setTransform(t);
    for (auto& tri : triangles)
        tri.setTransform(t);

    BoundingBoxd box = triangles[0].getBoundingBox();
    for (size_t i = 1; i < triangles.size(); i++)
        box = BoundingBoxd(box, triangles[i].getBoundingBox());

    setBoundingBox(box);
}


bool RT_Mesh::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    bool hit = false;
    double closest = t_interval.max;
    RT_Record tmp;

    for (const auto& tri : triangles) {
        if (tri.rayIntersect(ray, Intervald(t_interval.min, closest), tmp)) {
            closest = tmp.t;
            hit = true;
            rec = tmp;
        }
    }
    return hit;
}
*/