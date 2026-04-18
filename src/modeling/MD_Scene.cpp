#include "MD_Scene.h"

MD_Scene::MD_Scene() {}

MD_Scene::~MD_Scene() {
    for (auto* obj : objects) delete obj;
    for (auto* lgt : point_lights) delete lgt;
}

MD_Object* MD_Scene::createObject(MD_Shape* _shape, const TRSDataf& _transform, MD_Material* _mat) {
    MD_Object* obj = new MD_Object(_shape, _mat, _transform);
    objects.push_back(obj);
    return obj;
}

void MD_Scene::removeObject(int index) {
    if (index < 0 || index >= (int)objects.size()) return;
    delete objects[index];
    std::swap(objects[index], objects.back());
    objects.pop_back();
}

MD_Object* MD_Scene::getObject(int index) {
    if (index < 0 || index >= (int)objects.size()) return nullptr;
    return objects[index];
}

std::vector<MD_Object*>& MD_Scene::getObjects() { return objects; }

void MD_Scene::nextObject() {
    if (objects.empty()) return;
    selected_index = (selected_index + 1) % (int)objects.size();
}


// ── Point lights ─────────────────────────────────────────────────────────

MD_PointLight* MD_Scene::createPointLight(const Vec3f& position, const Color& color, float intensity) {
    MD_PointLight* light = new MD_PointLight(position, color, intensity);
    point_lights.push_back(light);
    return light;
}


void MD_Scene::removePointLight(int index) {
    if (index < 0 || index >= (int)point_lights.size()) return;
    delete point_lights[index];
    std::swap(point_lights[index], point_lights.back());
    point_lights.pop_back();
}

MD_PointLight* MD_Scene::getPointLight(int index) {
    if (index < 0 || index >= (int)point_lights.size()) return nullptr;
    return point_lights[index];
}

std::vector<MD_PointLight*>& MD_Scene::getPointLights() { return point_lights; }
