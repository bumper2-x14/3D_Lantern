#include "MD_Quad.h"


MD_Quad::MD_Quad(float _width =1 , float _lenght = 1){
    width = _width;
    length = _lenght;

    buildShape();
    mesh.setupMD_Mesh();
}

void MD_Quad::buildShape(){
    Vec3f p0 (-1, 0, -1);
    Vec3f p1 (-1, 0,  1);
    Vec3f p2 ( 1, 0, -1);
    Vec3f p3 ( 1, 0,  1);

    Vec3f n ( 0, 1, 1);

}