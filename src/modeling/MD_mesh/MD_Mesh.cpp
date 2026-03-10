#include <vector>

#include "../../assets/mesh_data.h"
#include "MD_Mesh.h"

void MD_Mesh::setupMD_Mesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    

}