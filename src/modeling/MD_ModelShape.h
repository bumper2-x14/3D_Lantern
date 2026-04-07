#ifndef MD_MODELSHAPE_H
#define MD_MODELSHAPE_H

#include "MD_Shape.h"
#include "assets/model.h" 

class MD_ModelShape: public  MD_Shape{
    public:
        void buildShape() override;

};
#endif

