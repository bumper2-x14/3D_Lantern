#ifndef MD_CYLINDER_H
#define MD_CYLINDER_H

#include "MD_Shape.h"
#include "assets/vertex.h"

class MD_Cylinder : public MD_Shape{
    public:
        MD_Cylinder(int _nb_seg = 25);
        void buildShape() override;
        void applyTransform(Transform* transform) override; 
        std::string serialize() const override;

    private:
        int nb_seg;
};

#endif 