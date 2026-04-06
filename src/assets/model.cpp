#include "model.h"
#include "tiny_obj_loader.h"

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

bool Model::loadOBJ(const std::string& path){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

    if (!warn.empty()) std::cout << "OBJ warning: " << warn << std::endl;
    if (!err.empty())  std::cerr << "OBJ error: "   << err  << std::endl;
    if (!ok) return false;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {

            Vec3f pos(
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2]
            );

            Vec3f normal(0, 0, 0);
            if (idx.normal_index >= 0) {
                normal = Vec3f(
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]
                );
            }

            Vec2f uv(0, 0);
            if (idx.texcoord_index >= 0) {
                uv = Vec2f(
                    attrib.texcoords[2 * idx.texcoord_index + 0],
                    attrib.texcoords[2 * idx.texcoord_index + 1]
                );
            }

            vertices.push_back(Vertex(pos, normal, uv));
            indices.push_back((unsigned int)indices.size());
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