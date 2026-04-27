#ifndef PERLIN_TEXTURE_H
#define PERLIN_TEXTURE_H

#include "texture.h"
#include "perlin.h"

/// @brief Different procedural styles based on Perlin noise.
enum class PerlinClassType {
    NOISE,
    TURBULENCE,
    MARBLE,
    WOOD,
    WATER,
    FIRE,
    ICE,
    TERRAIN,
    WARPED,
    STRIPES,
    CLOUD
};

/// @brief Procedural texture using Perlin noise.
///        Can generate different effects depending on the type.
class PerlinTexture : public Texture {
    public:
        /// @brief Creates a Perlin texture.
        /// @param _type Type of effect (marble, wood, etc).
        /// @param _scale Controls frequency/size of the pattern.
        PerlinTexture(PerlinClassType _type, double _scale);

        /// @brief Returns color based on noise and selected type.
        Color sample(const Vec2d& uv, const Point3d& p) const override;

        /// @brief Converts UV to world coordinates (used for procedural mapping).
        Point3d uvToWorldConverter(const Vec2d& uv) const override;

        /// @brief Serializes the texture into LNT format.
        std::string serializeLNT() const override;

    private:
        Perlin noise_gen; ///< Perlin noise generator.
        double scale; ///< Controls pattern scale.
        PerlinClassType type; ///< Selected procedural style.
};

#endif