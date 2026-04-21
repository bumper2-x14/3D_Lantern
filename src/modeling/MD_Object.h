#ifndef MD_OBJECT_H
#define MD_OBJECT_H


#include "modeling/MD_Shape.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Material.h"
#include "math/TRS_data.h"

class MD_Object {
    public:
        TRSDataf trs;
        bool selectable = true;
        std::string name;

        MD_Object(const std::string& _name, MD_Shape* shape = nullptr,
                        MD_Material* material = nullptr,
                                const TRSDataf& _trs_data = {});

        MD_Shape* getShape()const { return shape; }
        
        MD_Material* getMaterial()const { return material; }

        void setMaterial(MD_Material* m) { material = m; }

        Mat4f getTransformMatrix() const;

        void draw(MD_Shader& shader);

        std::string serialize() const;
        
    private:
        MD_Shape* shape;
        MD_Material* material;
};
#endif
