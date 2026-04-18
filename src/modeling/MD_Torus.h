#ifndef MD_TORUS_H
#define MD_TORUS_H

#include "MD_Shape.h"

class MD_Torus : public MD_Shape{
    public:
        MD_Torus(int _nb_ring = 25, int _nb_side=25);
        void buildShape() override;
        void applyTransform(Transform* transform) override; 
        std::string serialize() const override;

    private:
        int nb_ring; // divisons around the big circle
        int nb_side; // division around the small circlr
};


#endif 