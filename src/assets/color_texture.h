#ifndef COLOR_TEXTURE_H
#define COLOR_TEXTURE_H

#include "texture.h"

/// @brief Simple texture that always returns a constant color.
class ColorTexture : public Texture {
    public:
        /// @brief Creates a texture with a fixed color.
        /// @param _albedo Base color.
        ColorTexture(const Color& _albedo);

        /// @brief Returns the stored color.
        Color getColor() const { return albedo; }

        /// @brief Returns the color (ignores uv and position since it's constant).
        Color sample(const Vec2d& uv, const Point3d& p) const override;

    private:
        Color albedo; ///< Stored color.
};

#endif