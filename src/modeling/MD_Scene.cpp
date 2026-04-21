#include "MD_Scene.h"

MD_Scene::MD_Scene() {}

MD_Scene::~MD_Scene() {
    for (auto* obj : objects) delete obj;
    for (auto* lgt : point_lights) delete lgt;
}

MD_Object* MD_Scene::createObject(const std::string& _name, MD_Shape* _shape, const TRSDataf& _transform, MD_Material* _mat) {
    MD_Object* obj = new MD_Object(_name, _shape, _mat, _transform);
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
    selected_obj_index = (selected_obj_index + 1) % (int)objects.size();
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

void MD_Scene::loadDefaultScene() {
    // clear anything already in the scene
    for (auto* o : objects)     delete o;
    for (auto* l : point_lights) delete l;
    objects.clear();
    point_lights.clear();
    selected_obj_index = 0;

    // ground — not selectable
    MD_Object* ground = createObject("Ground", &default_ground,
                                     TRSDataf{ {0.f, 0.f, 0.f} },
                                     &default_ground_mat);
    ground->selectable = false;

    // sphere sitting on the ground
    createObject("Sphere", &default_sphere,
                 TRSDataf{ {0.f, 1.f, 0.f} },
                 &default_sphere_mat);

    // one warm point light above and to the side
    createPointLight(Vec3f(3.f, 4.f, 3.f), Color(1.f, 1.f, 1.f), 2.0f);
}

MD_Object* MD_Scene::getSelectedObject() const {
    if (selected_is_light || selected_obj_index < 0
        || selected_obj_index >= (int)objects.size()) return nullptr;
    return objects[selected_obj_index];
}

MD_PointLight* MD_Scene::getSelectedLight() const {
    if (!selected_is_light || selected_light_index < 0
        || selected_light_index >= (int)point_lights.size()) return nullptr;
    return point_lights[selected_light_index];
}

void MD_Scene::deselect() {
    show_selected = false;
    selected_is_light = false;
    selected_light_index = -1;
    selected_obj_index = -1;
}


void MD_Scene::setSelectedObject(int index) {
    selected_obj_index = index;
}

void MD_Scene::setSelectedPointLight(int index) {
    selected_light_index = index;
}