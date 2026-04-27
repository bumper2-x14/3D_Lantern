#ifndef SHARED_RESOURCES_H
#define SHARED_RESOURCES_H

#include "registry.h"
#include "image.h"
#include "model.h"
#include "texture.h" 

/// @brief Stores shared resources (images, models, textures).
///        Avoids loading the same file multiple times.
class SharedResources {
    public:
        // Images

        /// @brief Loads an image from file and stores it with a key.
        /// @param key Name used to retrieve the image.
        /// @param path Path to the image file.
        Image* loadImage(const std::string& key, const std::string& path) {
            return images.add(key, new Image(Image::load(path)));
        }

        /// @brief Returns image by key.
        Image* getImage(const std::string& key) const { return images.get(key); }

        /// @brief Checks if an image exists.
        bool hasImage(const std::string& key) const { return images.has(key); }

        // OBJ models

        /// @brief Loads a model from file and stores it.
        /// @param key Name used to retrieve the model.
        /// @param path Path to the model file.
        Model* loadModel(const std::string& key, const std::string& path) {
            Model* m = new Model();
            if (!m->loadModel(path)) { delete m; return nullptr; }
            return models.add(key, m);
        }

        /// @brief Returns model by key.
        Model* getModel(const std::string& key) const { return models.get(key); }

        /// @brief Checks if a model exists.
        bool hasModel(const std::string& key) const { return models.has(key); }

        // Textures

        /// @brief Adds a texture to the registry.
        Texture* addTexture(const std::string& key, Texture* tex) { return textures.add(key, tex); }

        /// @brief Returns texture by key.
        Texture* getTexture(const std::string& key) const { return textures.get(key); }

        /// @brief Checks if a texture exists.
        bool hasTexture(const std::string& key) const { return textures.has(key); }

        /// @brief Returns all stored textures.
        const std::unordered_map<std::string, Texture*>& getTextures() const {
            return textures.getAll();
        }

        /// @brief Finds the name associated with a texture pointer.
        std::string findTextureName(const Texture* ptr) const {
            for (const auto& [name, tex] : textures.getAll())
                if (tex == ptr) return name;
            return "";
        }
        
    private:
        Registry<Image> images; ///< Stored images.
        Registry<Model> models; ///< Stored models.
        Registry<Texture> textures; ///< Stored textures.
};

#endif