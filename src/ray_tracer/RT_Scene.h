#ifndef RT_SCENE_H
#define RT_SCENE_H

#include "RT_Object.h"
#include "RT_ObjectList.h"
#include "RT_Light.h"
#include "math/color.h"
#include <vector>

/// @brief Stores all objects and lights of the ray tracing scene.
class RT_Scene {
public:
    /// @brief Default constructor.
    RT_Scene() = default;

    /// @brief Destructor (cleans all allocated objects and lights).
    ~RT_Scene() {
        for (auto* o : objects) delete o;
        for (auto* l : lights)  delete l;
    }

    /// @brief Creates and adds a new object to the scene.
    /// @tparam T Object type.
    template<typename T, typename... Args>
    T* emplaceObject(Args&&... args){ 
        T* obj = new T(std::forward<Args>(args)...);
        objects.push_back(obj);
        return obj;
    }

    /// @brief Creates and adds a new light to the scene.
    /// @tparam T Light type.
    template<typename T, typename... Args>
    RT_Light* emplaceLight(Args&&... args){
        T* light = new T(std::forward<Args>(args)...); 
        lights.push_back(light); 
        return light;
    }

    /// @brief Returns list of objects.
    std::vector<RT_Object*>& getObjects() { return objects; }

    /// @brief Returns list of lights.
    const std::vector<RT_Light*>& getLights() const { return lights; }

private:
    std::vector<RT_Object*> objects; ///< All scene objects.
    std::vector<RT_Light*> lights; ///< All scene lights.
};

#endif