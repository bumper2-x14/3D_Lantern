#ifndef MODELING_RESOURCES_H
#define MODELING_RESOURCES_H

#include "assets/registry.h"
#include "modeling/MD_Material.h"
#include "modeling/MD_Shape.h"
#include "MD_ModelShape.h"

class ModelingResources {
public:
    // MD_Materials
    MD_Material* addMaterial(const std::string& key, MD_Material* mat) { return materials.add(key, mat); }
    MD_Material* getMaterial(const std::string& key) const  { return materials.get(key); }
    bool hasMaterial(const std::string& key) const  { return materials.has(key); }

    std::string findMaterialName(const MD_Material* ptr) const {
        for (auto& [name, mat] : materials.getAll())
            if (mat == ptr) return name;
        return "";
    }

    MD_ModelShape* addModelShape(const std::string& key, Model* model) {
        return shapes.add(key, new MD_ModelShape(model->getMesh()));
    }
    MD_ModelShape* getModelShape(const std::string& key) const { return shapes.get(key); }
    bool hasModelShape(const std::string& key) const { return shapes.has(key); }

    const std::unordered_map<std::string, MD_Material*>& getMaterials() const {
        return materials.getAll();
    }

private:
    Registry<MD_Material> materials;
    Registry<MD_ModelShape> shapes;
};

#endif