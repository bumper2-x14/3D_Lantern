#ifndef MD_RENDERER
#define MD_RENDERER

#include <vector> 
#include "MD_Object.h"

class renderer{
    public:
        
    private:
        std::vector<MD_Object> objects;
        MD_Camera camera;
        MD_Quad ground;
};
