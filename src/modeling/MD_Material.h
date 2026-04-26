#ifndef MD_MATERIAL_H
#define MD_MATERIAL_H

#include "MD_Shader.h"
#include "assets/texture.h"
#include "math/vec3.h"

/// @brief Represents a material used for rendering (color or texture + lighting info).
class MD_Material{
    public:
        static constexpr int TEX_RESOLUTION = 256; ///< Default resolution used for generated textures.

        /// @brief Different types of material behavior.
        enum class MatType {
            TEXTURE,  ///< Uses a texture.
            AMBIENT,  ///< Ambient-only color.
            DIFFUSE,  ///< Diffuse lighting.
            SPECULAR  ///< Specular lighting (with shininess).
        };

        /// @brief Creates a material using a texture.
        /// @param _texture Pointer to the texture.
        MD_Material(Texture* _texture);

        /// @brief Creates a material using a flat color.
        /// @param color Base color.
        /// @param type Type of material (ambient, diffuse, specular).
        /// @param shininess Specular exponent (used for lighting).
        MD_Material(const Vec3f& color, MatType type, float shininess = 32.f);

        /// @brief Destructor (frees GL resources if needed).
        ~MD_Material();

        // disable copy (material owns GPU resources)
        MD_Material(const MD_Material&) = delete;
        MD_Material& operator=(const MD_Material&) = delete;

        /// @brief Sends material data to the shader (texture or color + lighting).
        /// @param shader Shader used for rendering.
        void bind(const MD_Shader& shader) const;

        /// @brief Unbinds the material (cleanup state if needed).
        void unbind() const;

        /// @brief Returns the material type.
        MatType getType() const { return type; }

        /// @brief Checks if this material uses a texture.
        bool hasTexture() const { return type == MatType::TEXTURE; }

        /// @brief Returns the base color.
        Color getColor() const { return color; }

        /// @brief Returns the shininess value.
        float getShininess() const { return shininess; }

        /// @brief Returns the texture pointer.
        Texture* getTexture() const { return texture; }

        /// @brief Returns the OpenGL texture id.
        unsigned int getTextureId(){ return gl_texture_id; }

        /// @brief Updates the material color.
        void setColor(const Color& _color) { color = _color; }

    private:
        /// @brief Uploads the texture to the GPU.
        void uploadTexture();

        MatType type = MatType::DIFFUSE; ///< Type of material.
        Texture* texture = nullptr; ///< Pointer to texture (if any).
        Color color {}; ///< Base color.
        float shininess = 32.f; ///< Specular intensity.
        unsigned int gl_texture_id = 0; ///< OpenGL texture ID.
};

#endif