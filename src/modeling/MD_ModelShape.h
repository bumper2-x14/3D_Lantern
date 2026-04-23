#ifndef MD_MODELSHAPE_H
#define MD_MODELSHAPE_H

#include "MD_Shape.h"
#include "assets/mesh_data.h"
#include "assets/model.h" 

class MD_ModelShape: public MD_Shape {
    public:
        MD_ModelShape(MeshData* _mesh_data);
        void buildShape() override;
        void applyTransform(Transform* transform) override;

    private:
       std::string path;   
       MeshData* meshData = nullptr;
};
#endif

