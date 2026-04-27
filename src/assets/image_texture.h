#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "texture.h"
#include "image.h"

/// @brief Texture based on an image file.
class ImageTexture : public Texture {
    public:
        /// @brief Loads a texture from a file path.
        /// @param _path Path to the image.
        ImageTexture(const std::string& _path);

        /// @brief Creates a texture from an existing Image.
        /// @param _img Pointer to image data.
        /// @param name Optional name/path.
        ImageTexture(const Image* _img, const std::string& name = "");

        /// @brief Samples the image using UV coordinates.
        Color sample(const Vec2d& uv, const Point3d& p) const override;

        /// @brief Serializes the texture into LNT format.
        std::string serializeLNT() const override;

    private:
        std::string path; ///< Path to the image (if loaded from file).
        const Image* tex_img; ///< Pointer to image used for sampling.
        Image owned; ///< Local copy if we own the image.
};

#endif