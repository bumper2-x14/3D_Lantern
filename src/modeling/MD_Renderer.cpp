#include "MD_Renderer.h"

MD_Renderer::MD_Renderer()
    : camera_Main(0, 3, -3, 0, -2, 2)
    , camera_Rt  (0, 1, -3,  0, 0, 2)
    , groundShape(100, 100)
    , ground(&groundShape, nullptr, {})   // identity TRS — no translation needed
{}

MD_Renderer::MD_Renderer(const MD_Camera& _camera)
    : camera_Main(_camera)
    , camera_Rt  (0, 1, -3, 0, 0, 2)
    , groundShape(100, 100)
    , ground(&groundShape, nullptr, {})
{}

MD_Renderer::MD_Renderer(const MD_Camera& _camera_Main,
                         const MD_Camera& _camera_Rt)
    : camera_Main(_camera_Main)
    , camera_Rt  (_camera_Rt)
    , groundShape(100, 100)
    , ground(&groundShape, nullptr, {})
{}

void MD_Renderer::addObject(MD_Object* _object) {
    objects.push_back(_object);
}

void MD_Renderer::deleteObject(int indice) {
    objects.erase(objects.begin() + indice);
}

MD_Camera& MD_Renderer::setCameraMain() { return camera_Main; }
MD_Camera& MD_Renderer::setCameraRt()   { return camera_Rt;   }

void MD_Renderer::render(MD_Shader& shader) {
    for (auto* obj : objects)
        obj->draw(shader);
}