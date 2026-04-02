#include "model.h"

Model::Model(){};

Model::Model(const std::string& path){
    loadModel(path);
}

bool Model::loadModel(const std::string& path){
    if(! loadOBJ(path)) return false;

    return true;
}

// return private data member (mesh) 
const MeshData& Model::getMesh() const {
    return mesh;
}

MeshData& Model::getMesh() {
    return mesh;
}

bool Model::loadOBJ(const std::string& path){
    std::ifstream file(path);

    // testing the opening file
    if (!file.is_open()) {
        std::cout<<"Error: cannot open OBJ file: "<< path <<std::endl;
        return false; 
    }
    
    std::vector<Vec3f> positions;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> uvs;

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

        // normals
        else if( type == "vn"){
            float xn, yn, zn;
            file >> xn >> yn >> zn;
            normals.push_back(Vec3f(xn, yn, zn));
        }

        // texture
        else if( type == "vt"){
            float u, v;
            file >> u >> v;
            uvs.push_back(Vec2f(u, v));
        }

        // loading indices (ex: f 1/2/3)
        else if(type == "f"){
            int vIndice1, vIndice2, vIndice3;
            int tIndice1, tIndice2, tIndice3;
            int nIndice1, nIndice2, nIndice3;
            char slash;
            
            file >> vIndice1 >> slash >> tIndice1 >> slash >> nIndice1
                 >> vIndice2 >> slash >> tIndice2 >> slash >> nIndice2
                 >> vIndice3 >> slash >> tIndice3 >> slash >> nIndice3;
            
            Vec3f p1 = positions[vIndice1 - 1];
            Vec3f p2 = positions[vIndice2 - 1];
            Vec3f p3 = positions[vIndice3 - 1];

            Vec3f n1 = normals[nIndice1 - 1];
            Vec3f n2 = normals[nIndice2 - 1];
            Vec3f n3 = normals[nIndice3 - 1];

            Vec2f uv1 = uvs[tIndice1 - 1];
            Vec2f uv2 = uvs[tIndice2 - 1];
            Vec2f uv3 = uvs[tIndice3 - 1];


            Vertex v1(p1, n1, uv1);
            Vertex v2(p2, n2, uv2);
            Vertex v3(p3, n3, uv3);
            
            // add vertices and indices
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

    mesh = MeshData(vertices, indices);
    return true;

 }

void Model::regressionTest(){

    // loading file
    Model model("test.obj");

    MeshData meshTest = model.getMesh();

    // mesh should contain data
    assert(!meshTest.vertices.empty());
    assert(!meshTest.indices.empty());

    // triangle -> 3 vertices
    assert(meshTest.vertices.size() == 3);

    std::cout << "Model regression test passed"<<std::endl;
}