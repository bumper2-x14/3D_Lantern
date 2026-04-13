#ifndef MODEL_LIST_H
#define MODEL_LIST_H

#include <unordered_map>
#include <string>
#include "model.h"

/**
 * @class ModelList
 * @brief Owns and manages a collection of named Model objects.
 *
 * Models are loaded from disk on insertion and destroyed on removal or destruction.
 * Not copyable — use pointers or references to share access.
 *
 * @example
 * @code
 *   ModelList list;
 *   list.add("chair", "assets/chair.obj");
 *   Model* m = list.get("chair");
 * @endcode
 */
class ModelList {
public:
    ~ModelList() { clear(); }

    ModelList(const ModelList&) = delete;
    ModelList& operator=(const ModelList&) = delete;

    /**
     * @brief Loads a model from disk and stores it under a name.
     * @param name  Unique identifier for the model.
     * @param path  Path to the .obj file.
     * @return true on success, false if @p name already exists or load fails.
     */
    bool add(const std::string& name, const std::string& path);

    /**
     * @brief Retrieves a model by name.
     * @param name  Name the model was registered under.
     * @return Pointer to the model, or nullptr if not found.
     */
    Model* get(const std::string& name);
    const Model* get(const std::string& name) const; ///< @overload

    /// @brief Returns the mesh of a model by name, or nullptr if not found.
    const MeshData* getMesh(const std::string& name) const {
        const Model* m = get(name);
        return m ? &m->getMesh() : nullptr;
    }

    /**
     * @brief Deletes and removes a model by name.
     * @param name  Name of the model to remove.
     * @return true if found and removed, false otherwise.
     */
    bool remove(const std::string& name);

    /// @brief Returns true if a model is registered under @p name.
    bool has(const std::string& name) const;

    /// @brief Returns the number of loaded models.
    size_t size() const;

    /// @brief Deletes all models.
    void clear();

private:
    std::unordered_map<std::string, Model*> models;
};

#endif