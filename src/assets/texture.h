#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>

#include "math/color.h"
#include "math/vec2.h"
#include "math/point3.h"

/// @brief Base class for all textures.
///        Each texture must know how to return a color from UV and/or 3D position.
class Texture {
    public:
        /// @brief Virtual destructor for inherited texture classes.
        virtual ~Texture() = default;

        // Core interface — every subclass implements this
        // u, v : surface coordinates [0,1]
        // p    : 3D hit point (needed for solid/procedural textures)

        /// @brief Returns the color of the texture at given UV and 3D point.
        virtual Color sample(const Vec2d& uv, const Point3d& p) const = 0;

        // Maps a UV coordinate [0,1]² to a 3D world-space point on the XZ plane.
        // Used by toPixelBuffer() to rasterize procedural textures (checker, noise etc.)
        // that evaluate in world space rather than UV space.
        // The default maps UV to a 2x2 world unit patch centered at the origin:
        //   uv(0,0) → (-1, 0, -1)
        //   uv(1,1) → ( 1, 0,  1)
        // Subclasses should override this to match their procedural scale.

        /// @brief Converts UV coordinates to a 3D point for procedural textures.
        virtual Point3d uvToWorldConverter(const Vec2d& uv) const {
            return Point3d((uv.x - 0.5) * 2.0, 0.0, (uv.y - 0.5) * 2.0);
        }

        // Rasterizes the texture into a flat width×height pixel buffer.
        // Each pixel samples the texture at its corresponding UV coordinate,
        // converting the result to gamma-encoded 8-bit RGB via toRGB().
        //
        // The world-space point p passed to sample() is derived from
        // uvToWorldConverter() — UV textures (image, solid) ignore p entirely,
        // while procedural textures (checker, noise) use it for 3D evaluation.
        //
        // Gamma note: toRGB() applies gamma correction, which is correct for:
        //   - glTexImage2D with GL_RGB8 + GL_UNSIGNED_BYTE
        //   - PPM file output
        // Do NOT use GL_SRGB8 as the internal format — the driver would apply
        // gamma a second time on top of toRGB(), producing washed-out results.
        // Do NOT call this for intermediate/compositing textures (RT_MixTexture)
        // — blending must happen in linear space before gamma is applied.
        //
        // UV axes: i iterates height (v), j iterates width (u)
        // Buffer layout: buffer[i * width + j] — row-major, top to bottom

        /// @brief Converts the texture into a pixel buffer.
        ///        Useful to upload procedural textures to OpenGL or save them as images.
        std::vector<RGB> createPixelBuffer(int width, int height) const {
            if (width <= 0 || height <= 0) return {};
            std::vector<RGB> buffer(width * height);
            for (int i = 0; i < height; i++){
                for (int j = 0; j < width; j++){
                    Vec2d uv(
                        width  > 1 ? double(j) / (width  - 1) : 0.0,
                        height > 1 ? double(i) / (height - 1) : 0.0
                    );  
                    Point3d p = uvToWorldConverter(uv);
                    Color c = sample(uv, p);
                    buffer[i * width + j] = c.toRGB();
                }
            }
            return buffer;
        }

        /// @brief Serializes the texture into LNT format.
        virtual std::string serializeLNT() const { return ""; }
};

#endif