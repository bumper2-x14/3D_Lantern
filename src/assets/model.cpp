#include "model.h"

Model::Model(const std::string& path){
    loadModel(path);
}

bool Model::loadModel(const std::string& path){
    // clear previous data
    meshes.clear();

    // temp mesh
    MeshData mesh;
    if(! loadOBJ(path, mesh)) return false;

    meshes.push_back(mesh);
    return true;
}

// return private data member (mesh) 
const std::vector<MeshData>& Model::getMeshes() const {
    return meshes;
}

bool Model::loadOBJ(const std::string& path, MeshData& outMeshes){
    std::ifstream file(path);

    // testing the opening file
    if (!file.is_open()) {
        std::cout<<"Error: cannot open OBJ file: "<< path <<std::endl;
        return false; 
    }

    std::vector<Vec3f> positions;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::string type;

    // read file tokens
    while(file >> type){
        // vertex positin
        if (type == "v"){
            float x,y,z;
            file >> x  >> y >> z;
            positions.push_back(Vec3f(x, y, z));
        }

        // loading indices
        else if(type == "f"){
            int i, j, k;
            file >> i >> j >> k;
            
            Vec3f p1 = positions[i - 1];
            Vec3f p2 = positions[j - 1];
            Vec3f p3 = positions[k - 1];

            // normals
            Vec3f edge1 = p2 - p1;
            Vec3f edge2 = p3 - p1;
            Vec3f normal = normalize(cross(edge1, edge2));

            // add vertices and indices
            Vertex v1(p1, normal, Vec2f(0, 0));
            Vertex v2(p2, normal, Vec2f(0, 0));
            Vertex v3(p3, normal, Vec2f(0, 0));

            vertices.push_back(v1);
            indices.push_back((unsigned int)vertices.size() - 1);

            vertices.push_back(v2);
            indices.push_back((unsigned int)vertices.size() - 1);

            vertices.push_back(v3);
            indices.push_back((unsigned int)vertices.size() - 1);

        }
    }

    if (vertices.empty()) {
        std::cout << "Error: OBJ file contains no vertices ." ;
        return false;
    }

    outMeshes = MeshData(vertices, indices);
    return true;

 }

void Model::regressionTest(){

    // load valid file
    Model model("test.obj");

    const std::vector<MeshData>& meshes = model.getMeshes();

    // at least one mesh
    assert(meshes.size() >= 1);

    // mesh should contain data
    assert(!meshes[0].vertices.empty());
    assert(!meshes[0].indices.empty());

    // triangle -> 3 vertices
    assert(meshes[0].vertices.size() == 3);

    std::cout << "Model regression test passed"<<std::endl;
}