#ifndef MD_CIRCLE_H
#define MD_CIRCLE_H

#include "MD_Shape.h"
#include "assets/vertex.h"

class MD_Circle : public MD_Shape{
    public:
        MD_Circle(int _nb_seg = 25);
        void buildShape() override;
        void applyTransform(Transform* transform) override;

    private:
        int nb_seg;    

};

#endif