#ifndef MD_RENDERER_H
#define MD_RENDERER_H

#include <vector> 
#include "MD_Object.h"
#include "MD_Camera.h"
#include "MD_Shader.h"
#include "MD_Quad.h"
#include "math/transform.h"

class MD_Renderer{
    public:
        
        MD_Renderer();
        MD_Renderer(const MD_Camera& _camera);
        MD_Renderer(const MD_Camera& _camera_Main,
                        const MD_Camera& _camera_Rt);
        
        void addObject(MD_Object* _object);
        
        void deleteObject(int indice);
        
        MD_Camera& setCameraMain();
        MD_Camera& setCameraRt();
        

        void render(MD_Shader& shader);
        
    private:
        std::vector<MD_Object*> objects;
        MD_Camera camera_Main;
        MD_Camera camera_Rt;
        MD_Quad   groundShape;
        MD_Object ground ;
};

#endif
