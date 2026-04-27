#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <ostream>

#include "vec3.h"
#include "interval.h"

/// @brief Simple RGB struct (8-bit per channel).
struct RGB {
    unsigned char r, g, b;
};

/// @brief Color class (float RGB).
///        Used everywhere for rendering.
class Color {
    public:
        float r, g, b; ///< Color components.

        /// @brief Default constructor (black).
        Color() : r(0), g(0), b(0) {}

        /// @brief Creates a color from 3 floats.
        Color(float r, float g, float b) : r(r), g(g), b(b) {}

        /// @brief Creates a color from Vec3f.
        Color(const Vec3f& v) : r(v.x), g(v.y), b(v.z) {}

        /// @brief Add two colors.
        Color operator+(const Color& c) const { 
            return Color(r+c.r, g+c.g, b+c.b); 
        }

        /// @brief Subtract two colors.
        Color operator-(const Color& c) const { 
            return Color(r-c.r, g-c.g, b-c.b); 
        }

        /// @brief Multiply colors (component-wise).
        Color operator*(const Color& c) const { 
            return Color(r*c.r, g*c.g, b*c.b); 
        }

        /// @brief Multiply by scalar.
        Color operator*(float s) const {
            return Color(r*s, g*s, b*s); 
        }

        /// @brief Divide by scalar.
        Color operator/(float s) const {
            return Color(r/s, g/s, b/s);
        }

        /// @brief Add-assign.
        Color& operator+=(const Color& c) {
            r+=c.r;
            g+=c.g;
            b+=c.b; 
            return *this;
        }
        
        /// @brief Multiply-assign.
        Color& operator*=(float s) {
            r*=s;
            g*=s;
            b*=s;   
            return *this; 
        }

        /// @brief Subtract-assign.
        Color operator-=(const Color& c) { 
            r-=c.r; g-=c.g; b-=c.b; 
            return *this; 
        }

        /// @brief Divide-assign.
        Color operator/=(float s) {
             r/=s; g/=s; b/=s; 
             return *this; 
        }

        /// @brief Assign operator.
        Color& operator=(const Color& col) {
            r = col.r; g = col.g; b = col.b;
            return *this;
        }

        /// @brief Linear interpolation between two colors.
        static Color lerp(const Color& a, const Color& b, float t) {
            return a * (1.0f - t) + b * t;
        }

        /// @brief Clamps color between [0,1].
        Color clamp() const {
            Intervalf in(0.0, 1.0);
            return Color(
                in.clamp(r),
                in.clamp(g),
                in.clamp(b)
            );
        }

        /// @brief Converts to gamma space.
        Color toGamma(float gamma = 2.2f) const {
            float inv = 1.0f / gamma;
            return Color(
                std::pow(std::max(r, 0.0f), inv),
                std::pow(std::max(g, 0.0f), inv),
                std::pow(std::max(b, 0.0f), inv)
            );
        }

        /// @brief Converts to linear space.
        Color toLinear(float gamma = 2.2f) const {
            return Color(
                std::pow(std::max(r, 0.0f), gamma),
                std::pow(std::max(g, 0.0f), gamma),
                std::pow(std::max(b, 0.0f), gamma)
            );
        }

        /// @brief Converts to 8-bit RGB.
        RGB toRGB(float gamma = 2.2f) const {
            Color gcol = toGamma(gamma).clamp();
            return {
                static_cast<unsigned char>(gcol.r * 255.0f),
                static_cast<unsigned char>(gcol.g * 255.0f),
                static_cast<unsigned char>(gcol.b * 255.0f)
            };
        }            
};

/// @brief Converts Color to Vec3.
template<typename T>
inline Vec3<T>toVec3(const Color& col) { return Vec3<T>((T)col.r, (T)col.g, (T)col.b); } 

/// @brief Scalar * Color.
inline Color operator*(float s, const Color& c) { return c * s; }

/// @brief Replaces NaN values with 0.
inline float checkNaN(float v) { return std::isnan(v) ? 0.0f : v; }

/// @brief Writes color to output stream (PPM style).
inline void writeColor(std::ostream& out, const Color& color) {
    Color safe(checkNaN(color.r), checkNaN(color.g), checkNaN(color.b));
    RGB c = safe.toRGB();
    out << (int)c.r << ' ' << (int)c.g << ' ' << (int)c.b << '\n';
}

#endif