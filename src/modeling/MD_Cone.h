#ifndef MD_CONE_H
#define MD_CONE_H

#include "MD_Shape.h"


class MD_Cone : public MD_Shape{
    public:
        MD_Cone(int _nb_seg = 25);
        void buildShape() override;
        void applyTransform(Transform* transform) override; 
        std::string serialize() const override;

    private:
        int nb_seg;
};


#endif 