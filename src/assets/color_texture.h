#ifndef COLOR_TEXTURE_H
#define COLOR_TEXTURE_H

#include "texture.h"

class ColorTexture : public Texture {
    public:
        ColorTexture(const Color& _albedo);

        Color sample(const Vec2d& uv, const Point3d& p) const override;

    private:
        Color albedo;
};

#endif