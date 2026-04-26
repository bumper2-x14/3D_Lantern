#ifndef PERLIN_TEXTURE_H
#define PERLIN_TEXTURE_H

#include "texture.h"
#include "perlin.h"

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

class PerlinTexture : public Texture {
    public:
        PerlinTexture(PerlinClassType _type, double _scale);
        Color sample(const Vec2d& uv, const Point3d& p) const override;
        Point3d uvToWorldConverter(const Vec2d& uv) const override;
        std::string serializeLNT() const override;
    private:
        Perlin noise_gen;
        double scale;
        PerlinClassType type;
};

#endif