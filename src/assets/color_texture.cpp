#include "color_texture.h"

ColorTexture::ColorTexture(const Color& _albedo) : albedo(_albedo) {}

Color ColorTexture::sample(const Vec2d& uv, const Point3d& p) const {
    return albedo;
}