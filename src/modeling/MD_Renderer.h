#ifndef MD_RENDERER_H
#define MD_RENDERER_H

#include "MD_Scene.h"
#include "MD_Camera.h"
#include "MD_Shader.h"

class MD_Renderer {
public:
    MD_Renderer();

    // cameras
    MD_Camera& getCameraMain() { return camera_main; }
    // draw 
    void render(MD_Scene& scene, MD_Shader& shader);

private:
    void drawObject(MD_Object* obj, MD_Shader& shader, const Mat4f& view, const Mat4f& proj);

    MD_Camera camera_main;
};

#endif