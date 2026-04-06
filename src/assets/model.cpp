#include "model.h"
#include "tiny_obj_loader.h"
#include <unordered_map>

Model::Model() {}

Model::Model(const std::string& path){
    loadModel(path);
}

Model::~Model() {
    if (mesh != nullptr) 
        delete mesh;
}

Model::Model(Model&& other) noexcept
    : mesh(other.mesh) {
    other.mesh = nullptr;
}

Model& Model::operator=(Model&& other) noexcept {
    if (this != &other) {
        delete mesh;           // free current mesh
        mesh = other.mesh;     // steal the pointer
        other.mesh = nullptr;  
    }
    return *this;
}

bool Model::loadModel(const std::string& path){
    if(!loadOBJ(path)) return false;
    return true;
}

// return private data member (mesh) 
const MeshData& Model::getMesh() const {
    assert(mesh != nullptr && "getMesh() called before loading OBJ");
    return *mesh;
}

MeshData& Model::getMesh() {
    assert(mesh != nullptr && "getMesh() called before loading OBJ");
    return *mesh;
}


bool Model::loadOBJ(const std::string& path) {
    std::string dir = path.substr(0, path.find_last_of("/\\") + 1);
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), dir.c_str());
    if (!warn.empty()) std::cout << "OBJ warning: " << warn << std::endl;
    if (!err.empty())  std::cerr << "OBJ error: "   << err  << std::endl;
    if (!ok) return false;

    bool has_normals = !attrib.normals.empty();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<uint64_t, unsigned int> index_map;

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            uint64_t key = ((uint64_t)(unsigned int)idx.vertex_index << 32)
                         | (unsigned int)(idx.texcoord_index < 0 ? 0 : idx.texcoord_index);

            auto it = index_map.find(key);
            if (it == index_map.end()) {
                Vec3f pos(
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                );
                Vec3f normal(0, 0, 0);
                if (has_normals && idx.normal_index >= 0)
                    normal = Vec3f(
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    );
                Vec2f uv(0, 0);
                if (idx.texcoord_index >= 0)
                    uv = Vec2f(
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        attrib.texcoords[2 * idx.texcoord_index + 1]
                    );

                unsigned int new_idx = (unsigned int)vertices.size();
                vertices.push_back(Vertex(pos, normal, uv));
                index_map[key] = new_idx;
                indices.push_back(new_idx);
            } else {
                indices.push_back(it->second);
            }
        }
    }

    // compute and accumulate face normals only if OBJ had none
    if (!has_normals) {
        for (size_t i = 0; i + 2 < indices.size(); i += 3) {
            Vertex& a = vertices[indices[i + 0]];
            Vertex& b = vertices[indices[i + 1]];
            Vertex& c = vertices[indices[i + 2]];

            Vec3f e1(b.position.x - a.position.x,
                     b.position.y - a.position.y,
                     b.position.z - a.position.z);
            Vec3f e2(c.position.x - a.position.x,
                     c.position.y - a.position.y,
                     c.position.z - a.position.z);
            Vec3f fn(e1.y*e2.z - e1.z*e2.y,
                     e1.z*e2.x - e1.x*e2.z,
                     e1.x*e2.y - e1.y*e2.x);

            a.normal.x += fn.x; a.normal.y += fn.y; a.normal.z += fn.z;
            b.normal.x += fn.x; b.normal.y += fn.y; b.normal.z += fn.z;
            c.normal.x += fn.x; c.normal.y += fn.y; c.normal.z += fn.z;
        }

        for (auto& v : vertices) {
            float len = std::sqrt(v.normal.x*v.normal.x +
                                  v.normal.y*v.normal.y +
                                  v.normal.z*v.normal.z);
            if (len > 1e-8f) {
                v.normal.x /= len;
                v.normal.y /= len;
                v.normal.z /= len;
            }
        }
    }

    if (vertices.empty()) {
        std::cerr << "Error: no geometry found in " << path << std::endl;
        return false;
    }

    delete mesh;
    mesh = new MeshData(vertices, indices);
    return true;
}

void Model::regressionTest() {
    const std::string validPath   = std::string(RESOURCE_DIR) + "teapot.obj";
    const std::string invalidPath = "nonexistent.obj";

    // --- Test 1: default constructor, no mesh ---
    Model empty;
    assert(!empty.hasMesh() && "Default model should have no mesh");

    // --- Test 2: loading a valid OBJ ---
    Model model(validPath);
    assert(model.hasMesh() && "Model should have mesh after loading");
    const MeshData& mesh = model.getMesh();
    assert(!mesh.vertices.empty() && "Mesh should have vertices");
    assert(!mesh.indices.empty()  && "Mesh should have indices");
    assert(mesh.vertices.size() % 3 == 0 && "Vertices should be in triangles");
    assert(mesh.indices.size() == mesh.vertices.size() && "Indices should match vertices");

    // --- Test 3: loading an invalid path ---
    Model bad;
    bool loaded = bad.loadModel(invalidPath);
    assert(!loaded        && "Loading invalid path should return false");
    assert(!bad.hasMesh() && "Failed load should leave mesh null");

    // --- Test 4: move constructor ---
    Model moved = std::move(model);
    assert( moved.hasMesh() && "Moved-to model should own the mesh");
    assert(!model.hasMesh() && "Moved-from model should have no mesh");

    // --- Test 5: move assignment ---
    Model assigned;
    assigned = std::move(moved);
    assert( assigned.hasMesh() && "Assigned-to model should own the mesh");
    assert(!moved.hasMesh()    && "Assigned-from model should have no mesh");

    // --- Test 6: self-assignment guard ---
    assigned = std::move(assigned);
    assert(assigned.hasMesh() && "Self-assignment should leave mesh intact");

    // --- Test 7: reload OBJ on same model ---
    Model reloaded(validPath);
    bool reloadOk = reloaded.loadModel(validPath);
    assert(reloadOk           && "Reloading OBJ should succeed");
    assert(reloaded.hasMesh() && "Reloaded model should still have mesh");

    std::cout << "All Model regression tests passed" << std::endl;
}