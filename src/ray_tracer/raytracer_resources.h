#ifndef RAYTRACER_REGISTRY_H
#define RAYTRACER_REGISTRY_H

#include "assets/registry.h"
#include "ray_tracer/RT_Material.h"

/// @brief Stores and manages ray tracer materials (by name).
class RaytracerResources {
public:
    /// @brief Adds a material with a given key.
    RT_Material* addMaterial(const std::string& key, RT_Material* mat) { return materials.add(key, mat); }

    /// @brief Retrieves a material by key.
    RT_Material* getMaterial(const std::string& key) const { return materials.get(key); }

    /// @brief Checks if a material exists.
    bool hasMaterial(const std::string& key) const { return materials.has(key); }

private:
    Registry<RT_Material> materials; ///< Internal storage of materials.
};

#endif