#include "perlin_texture.h"


PerlinTexture::PerlinTexture(PerlinClassType _type, double _scale) : type(_type), scale(_scale) {}

Color PerlinTexture::sample(const Vec2d& uv, const Point3d& p) const {
    Point3d scaled_p = p * scale;

    switch(type) {
        case PerlinClassType::NOISE:
                return Color(1,1,1) * 0.5 * (1 + noise_gen.noise(scaled_p));
            
        case PerlinClassType::TURBULENCE:
                return Color(1,1,1) * noise_gen.turbulence(scaled_p, 7);
        
        case PerlinClassType::MARBLE: {
                return Color(1,1,1) * 0.5 *
                        (1 + sin(scaled_p.z + 10 * noise_gen.turbulence(scaled_p, 7)));
        }    
        case PerlinClassType::WOOD: {
                double r = sqrt(scaled_p.x*scaled_p.x + scaled_p.z*scaled_p.z);
                double rings = sin(r * 10.0 + 4.0 * noise_gen.turbulence(scaled_p, 7));
                double val = 0.5 * (1.0 + rings);

                // Interpolate between dark grain and light wood
                Color dark_grain (0.25, 0.12, 0.04);
                Color light_wood (0.70, 0.45, 0.20);
                return dark_grain + (light_wood - dark_grain) * val;
        }
        case PerlinClassType::WATER: {
                double wave = sin(scaled_p.x * 4.0 + 
                      5.0 * noise_gen.turbulence(scaled_p, 5));
                double val = 0.5 * (1 + wave);
                return Color(0.1, 0.3, 0.6) * val;
        }
        case PerlinClassType::FIRE: {
                double t = noise_gen.turbulence(scaled_p, 7);
                double f = sin(scaled_p.z + 8.0 * t);
                double val = 0.5 * (1 + f);

                return Color(1.0, val * 0.5, 0.0);
        }
        case PerlinClassType::ICE: {
                double t = noise_gen.turbulence(scaled_p, 10);
                if (t > 0.6)
                        return Color(1.0, 1.0, 1.0);
                else
                        return Color(0.6, 0.8, 1.0);
        }
        case PerlinClassType::TERRAIN: {
                double h = noise_gen.turbulence(scaled_p, 7);

                if (h < 0.3) return Color(0.2, 0.7, 0.3);   // grass
                if (h < 0.7) return Color(0.5, 0.4, 0.2);   // dirt
                return Color(1.0, 1.0, 1.0);               // snow
        }
        case PerlinClassType::WARPED: {
                Point3d q = scaled_p + 0.5 * Vec3d(
                        noise_gen.noise(scaled_p),
                        noise_gen.noise(scaled_p + Vec3d(5,5,5)),
                        noise_gen.noise(scaled_p + Vec3d(10,10,10))
                );

                double n = noise_gen.noise(q);
                return Color(1,1,1) * 0.5 * (1 + n);
        }
        case PerlinClassType::STRIPES: {
                double s = sin(scaled_p.x * 10.0);
                double val = 0.5 * (1 + s);

                return Color(val, val, val);
        }
        case PerlinClassType::CLOUD: {
                Point3d p = Point3d(
                        scaled_p.x * 1.5,
                        scaled_p.y * 0.5,
                        scaled_p.z
                );

                Point3d warp = p + 0.8 * Vec3d(
                        noise_gen.noise(p * 1.5),
                        noise_gen.noise(p * 1.5 + Vec3d(5.2, 1.3, 7.1)),
                        noise_gen.noise(p * 1.5 + Vec3d(8.3, 2.8, 3.4))
                );

                double t = noise_gen.turbulence(warp, 7);

                double density = std::max(0.0, t - 0.4);
                density /= (1.0 - 0.4);

                // sharper cloud edges
                density = density * density;

                // HEIGHT FALLOFF (flat cloud layer)
                double heightFalloff = exp(-p.y * 1.5);
                density *= heightFalloff;

                // clamp (safety)
                density = std::min(1.0, std::max(0.0, density));

                Color cloud_white(1.0, 1.0, 1.0);
                Color cloud_grey (0.7, 0.75, 0.8);

                Color col = cloud_white * density + cloud_grey * (1.0 - density);

                return col * density;
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

std::string PerlinTexture::serializeLNT() const {
    std::string t;
    switch (type) {
        case PerlinClassType::MARBLE:      t = "marble";      break;
        case PerlinClassType::WOOD:        t = "wood";        break;
        case PerlinClassType::TURBULENCE:  t = "turbulence";  break;
        case PerlinClassType::NOISE:       t = "noise";       break;
        case PerlinClassType::WARPED:      t = "warped";      break;
    }
    return "    type " + t + "\n    scale " + std::to_string(scale) + "\n";
}