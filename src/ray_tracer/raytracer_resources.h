#ifndef RAYTRACER_REGISTRY_H
#define RAYTRACER_REGISTRY_H

#include "assets/registry.h"
#include "ray_tracer/RT_Material.h"

class RaytracerResources {
public:
    RT_Material* addMaterial(const std::string& key, RT_Material* mat) { return materials.add(key, mat); }
    RT_Material* getMaterial(const std::string& key) const { return materials.get(key); }
    bool hasMaterial(const std::string& key) const { return materials.has(key); }

private:
    Registry<RT_Material> materials;
};

#endif