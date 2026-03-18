#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <cstdint>
#include <ostream>

#include "vec3.h"
#include "interval.h"


struct RGB {
    uint8_t r, g, b;
};

class Color {
    public:
        float r, g, b;

        Color() : r(0), g(0), b(0) {}
        Color(float r, float g, float b) : r(r), g(g), b(b) {}
        Color(const Vec3f& v) : r(v.x), g(v.y), b(v.z) {}

        Color operator+(const Color& c) const { 
            return Color(r+c.r, g+c.g, b+c.b); 
        }

        Color operator*(const Color& c) const { 
            return Color(r*c.r, g*c.g, b*c.b); 
        }
        Color operator*(float s) const {
            return Color(r*s, g*s, b*s); 
        }

        Color operator/(float s) const {
            return Color(r/s, g/s, b/s);
        }

        Color& operator+=(const Color& c) {
            r+=c.r;
            g+=c.g;
            b+=c.b; 
            return *this;
        }
        
        Color& operator*=(float s) {
            r*=s;
            g*=s;
            b*=s;   
            return *this; 
        }

        Color operator-=(const Color& c) { 
            r-=c.r; g-=c.g; b-=c.b; 
            return *this; 
        }

        Color operator/=(float s) {
             r/=s; g/=s; b/=s; 
             return *this; 
        }

        static Color lerp(const Color& a, const Color& b, float t) {
            return a * (1.0f - t) + b * t;
        }

        Color clamp() const {
            Intervalf in(0.0, 1.0);
            return Color(
                in.clamp(r),
                in.clamp(g),
                in.clamp(b)
            );
        }

        Color toGamma(float gamma = 2.2f) const {
            float inv = 1.0f / gamma;
            return Color(
                std::pow(std::max(r, 0.0f), inv),
                std::pow(std::max(g, 0.0f), inv),
                std::pow(std::max(b, 0.0f), inv)
            );
        }

        Color toLinear(float gamma = 2.2f) const {
            return Color(
                std::pow(std::max(r, 0.0f), gamma),
                std::pow(std::max(g, 0.0f), gamma),
                std::pow(std::max(b, 0.0f), gamma)
            );
        }

        RGB toRGB(float gamma = 2.2f) const {
            Color gcol = toGamma(gamma).clamp();
            return {
                static_cast<uint8_t>(gcol.r * 255.0f),
                static_cast<uint8_t>(gcol.g * 255.0f),
                static_cast<uint8_t>(gcol.b * 255.0f)
            };
        }            
};

inline Color operator*(float s, const Color& c) { return c * s; }

inline float checkNaN(float v) { return std::isnan(v) ? 0.0f : v; }

inline void writeColor(std::ostream& out, const Color& color) {
    Color safe(checkNaN(color.r), checkNaN(color.g), checkNaN(color.b));
    RGB c = safe.toRGB();
    out << (int)c.r << ' ' << (int)c.g << ' ' << (int)c.b << '\n';
}

#endif