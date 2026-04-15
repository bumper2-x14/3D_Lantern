#include "MD_Material.h"

MD_Material::MD_Material(Texture* t)
    : type(MatType::TEXTURE), texture(t) {
    uploadTexture();
}

MD_Material::MD_Material(const Vec3f& _color, MatType _type, float _shininess)
    : type(_type), color(_color), shininess(_shininess) {
    if (type == MatType::TEXTURE) type = MatType::DIFFUSE; // guard against misuse
}


MD_Material::~MD_Material() {
    if (gl_texture_id) glDeleteTextures(1, &gl_texture_id);
}

void MD_Material::uploadTexture() {
    if(!texture) return;

    std::vector<RGB> buffer = texture->createPixelBuffer(TEX_RESOLUTION, TEX_RESOLUTION);

    glGenTextures(1, &gl_texture_id);
    glBindTexture(GL_TEXTURE_2D, gl_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,
                 TEX_RESOLUTION, TEX_RESOLUTION, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MD_Material::bind(const MD_Shader& shader) const {
    shader.setInt("u_materialType", static_cast<int>(type));

    switch(type) {
        case MatType::TEXTURE:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gl_texture_id);
            shader.setInt("u_texture", 0);
            break;
        case MatType::AMBIENT:
        case MatType::DIFFUSE:
            shader.setVec3("u_color", color);
            break;
        case MatType::SPECULAR:
            shader.setVec3("u_color", color);
            shader.setFloat("u_shininess", shininess);
            break;
    }
}

void MD_Material::unbind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}