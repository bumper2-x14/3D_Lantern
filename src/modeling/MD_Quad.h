#ifndef MD_QUAD_H
#define MD_QUAD_H

#include "MD_Shape.h"
#include "assets/vertex.h"

class MD_Quad : public MD_Shape{
    public:
        MD_Quad(float _width =1 , float _lenght = 1);
        void buildShape() override;
        void applyTransform(Transform* transform) override;

    private:
        float width;
        float length;    
};


#endif 