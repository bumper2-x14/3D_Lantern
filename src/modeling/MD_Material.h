#ifndef MD_MATERIAL_H
#define MD_MATERIAL_H

#include "MD_Texture.h"
#include "math/vec3.h"

class MD_Material{
    public:
        static constexpr int TEX_RESOLUTION = 256;

        enum class MatType {
            TEXTURE,
            AMBIENT,
            DIFFUSE,
            SPECULAR
        };


        MD_Material(Texture* _texture);
        MD_Material(const Vec3f& color, MatType type, float shininess = 32.f);

        ~MD_Material();

        MD_Material(const MD_Material&) = delete;
        MD_Material& operator=(const MD_Material&) = delete;

        // Binds this material's GL texture (or flat color uniforms) to a shader
        void bind(const MD_Shader& shader) const;
        void unbind() const;

        MatType getType() const { return type; }
        bool hasTexture() const { return type == MatType::TEXTURE; }
        Color getColor() const { return color; }
        float getShininess() const { return shininess; }
        unsigned int getTextureId(){ return gl_texture_id; }

        void setColor(const Color& _color) { color = _color; }

    private:
        void uploadTexture();

        MatType type = MatType::DIFFUSE;
        Texture* texture = nullptr;
        Color color {};
        float shininess = 32.f;
        unsigned int gl_texture_id = 0;
};

#endif
