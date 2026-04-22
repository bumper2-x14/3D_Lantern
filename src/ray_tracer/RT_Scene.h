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
    ~RT_Scene() {
        for (auto* o : objects) delete o;
        for (auto* l : lights)  delete l;
    }

    template<typename T, typename... Args>
    T* emplaceObject(Args&&... args){ 
        T* obj = new T(std::forward<Args>(args)...);
        objects.push_back(obj);
        return obj;
    }

    template<typename T, typename... Args>
    RT_Light* emplaceLight(Args&&... args){
        T* light = new T(std::forward<Args>(args)...); 
        lights.push_back(light); 
        return light;
    }

    std::vector<RT_Object*>& getObjects() { return objects; }
    const std::vector<RT_Light*>& getLights() const { return lights; }

private:
    std::vector<RT_Object*> objects;
    std::vector<RT_Light*> lights;
};

#endif