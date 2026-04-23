#ifndef SHARED_RESOURCES_H
#define SHARED_RESOURCES_H

#include "registry.h"
#include "image.h"
#include "model.h"
#include "texture.h" 

class SharedResources {
    public:
        // Images
        Image* loadImage(const std::string& key, const std::string& path) {
            return images.add(key, new Image(Image::load(path)));
        }

        Image* getImage(const std::string& key) const { return images.get(key); }
        bool hasImage(const std::string& key) const { return images.has(key); }

        // OBJ models
        Model* loadModel(const std::string& key, const std::string& path) {
            Model* m = new Model();
            if (!m->loadModel(path)) { delete m; return nullptr; }
            return models.add(key, m);
        }

        Model* getModel(const std::string& key) const { return models.get(key); }
        bool hasModel(const std::string& key) const { return models.has(key); }

        // Textures
        Texture* addTexture(const std::string& key, Texture* tex) { return textures.add(key, tex); }
        Texture* getTexture(const std::string& key) const { return textures.get(key); }
        bool hasTexture(const std::string& key) const { return textures.has(key); }

        const std::unordered_map<std::string, Texture*>& getTextures() const {
            return textures.getAll();
        }
        std::string findTextureName(const Texture* ptr) const {
            for (const auto& [name, tex] : textures.getAll())
                if (tex == ptr) return name;
            return "";
        }
        
    private:
        Registry<Image> images;
        Registry<Model> models;
        Registry<Texture> textures;
};

#endif
