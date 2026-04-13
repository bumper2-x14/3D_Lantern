#ifndef MD_MODELSHAPE_H
#define MD_MODELSHAPE_H

#include "MD_Shape.h"
#include "assets/model.h" 

class MD_ModelShape: public  MD_Shape{
    public:
        MD_ModelShape(const std::string& _path);
        void buildShape() override;

    private:
       std::string path;   
};
#endif

