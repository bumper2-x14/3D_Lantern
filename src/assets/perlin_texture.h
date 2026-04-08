#ifndef PERLIN_TEXTURE_H
#define PERLIN_TEXTURE_H

#include "texture.h"
#include "perlin.h"

enum PerlinType {
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
        PerlinTexture(PerlinType _type, double _scale);
        Color sample(const Vec2d& uv, const Point3d& p) const override;
        Point3d uvToWorldConverter(const Vec2d& uv) const override;
    private:
        Perlin noise_gen;
        double scale;
        PerlinType type;
};

#endif