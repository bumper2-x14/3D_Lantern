#ifndef RT_SCENE_H
#define RT_SCENE_H

#include "RT_Object.h"
#include "RT_ObjectList.h"
#include "RT_Light.h"
#include "math/color.h"
#include <vector>

class RT_Scene {
public:
    RT_Scene() = default;
    ~RT_Scene() = default;

    void addObject(RT_Object* obj)  { objects.add(obj); }
    void addLight(RT_Light* light)  { lights.push_back(light); }

    RT_ObjectList& getObjects() { return objects; }
    const std::vector<RT_Light*>& getLights() const { return lights; }

private:
    RT_ObjectList objects;
    std::vector<RT_Light*> lights;
};

#endif