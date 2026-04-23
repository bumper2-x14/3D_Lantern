#ifndef MODELING_RESOURCES_H
#define MODELING_RESOURCES_H

#include "assets/registry.h"
#include "modeling/MD_Material.h"
#include "modeling/MD_Shape.h"

class ModelingResources {
public:
    // MD_Materials
    MD_Material* addMaterial(const std::string& key, MD_Material* mat) { return materials.add(key, mat); }
    MD_Material* getMaterial(const std::string& key) const  { return materials.get(key); }
    bool hasMaterial(const std::string& key) const  { return materials.has(key); }

    const std::unordered_map<std::string, MD_Material*>& getMaterials() const {
        return materials.getAll();
    }

private:
    Registry<MD_Material> materials;
};

#endif