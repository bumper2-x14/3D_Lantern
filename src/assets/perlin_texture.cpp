#include "perlin_texture.h"


PerlinTexture::PerlinTexture(PerlinType _type, double _scale) : type(_type), scale(_scale) {}

Color PerlinTexture::sample(const Vec2d& uv, const Point3d& p) const {
    Point3d scaled_p = p * scale;

    switch(type) {
        case NOISE:
                return Color(1,1,1) * 0.5 * (1 + noise_gen.noise(scaled_p));
            
        case TURBULENCE:
                return Color(1,1,1) * noise_gen.turbulence(scaled_p, 7);
        
        case MARBLE: {
                return Color(1,1,1) * 0.5 *
                        (1 + sin(scaled_p.z + 10 * noise_gen.turbulence(scaled_p, 7)));
            }
            
        case WOOD: {
                double r = sqrt(scaled_p.x*scaled_p.x + scaled_p.z*scaled_p.z);
                double rings = sin(r * 10.0 + 4.0 * noise_gen.turbulence(scaled_p, 7));
                double val = 0.5 * (1.0 + rings);

                // Interpolate between dark grain and light wood
                Color dark_grain (0.25, 0.12, 0.04);
                Color light_wood (0.70, 0.45, 0.20);
                return dark_grain + (light_wood - dark_grain) * val;
            }
    }

    return Color(0.0, 0.0, 0.0);
}

Point3d PerlinTexture::uvToWorldConverter(const Vec2d& uv) const {
    return Point3d(
            (uv.x - 0.5) * 4.0, // larger area = more visible detail
            0.0,
            (uv.y - 0.5) * 4.0
        );
}