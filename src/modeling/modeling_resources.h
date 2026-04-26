#ifndef MODELING_RESOURCES_H
#define MODELING_RESOURCES_H

#include "assets/registry.h"
#include "modeling/MD_Material.h"
#include "modeling/MD_Shape.h"
#include "MD_ModelShape.h"

/// @brief Stores and manages shared resources (materials + model shapes).
///        Works like a small registry using string keys.
class ModelingResources {
public:
    // MD_Materials

    /// @brief Adds a material to the registry.
    /// @param key Name used to store/retrieve the material.
    /// @param mat Pointer to the material.
    MD_Material* addMaterial(const std::string& key, MD_Material* mat) { return materials.add(key, mat); }

    /// @brief Returns a material by its key.
    MD_Material* getMaterial(const std::string& key) const  { return materials.get(key); }

    /// @brief Checks if a material exists.
    bool hasMaterial(const std::string& key) const  { return materials.has(key); }

    /// @brief Finds the name associated with a material pointer.
    /// @param ptr Pointer to the material.
    /// @return Key (name) or empty string if not found.
    std::string findMaterialName(const MD_Material* ptr) const {
        for (auto& [name, mat] : materials.getAll())
            if (mat == ptr) return name;
        return "";
    }

    /// @brief Adds a model shape from a loaded model.
    /// @param key Name used to store the shape.
    /// @param model Model containing mesh data.
    MD_ModelShape* addModelShape(const std::string& key, Model* model) {
        return shapes.add(key, new MD_ModelShape(model->getMesh()));
    }

    /// @brief Returns a model shape by its key.
    MD_ModelShape* getModelShape(const std::string& key) const { return shapes.get(key); }

    /// @brief Checks if a model shape exists.
    bool hasModelShape(const std::string& key) const { return shapes.has(key); }

    /// @brief Returns all registered materials.
    const std::unordered_map<std::string, MD_Material*>& getMaterials() const {
        return materials.getAll();
    }

private:
    Registry<MD_Material> materials; ///< Storage for materials.
    Registry<MD_ModelShape> shapes;  ///< Storage for model shapes.
};

#endif