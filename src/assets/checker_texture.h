#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H

#include "texture.h"

/// @brief Checker texture (like a chessboard), alternates between two textures or colors.
class CheckerTexture : public Texture {
    public:
        /// @brief Creates a checker texture using two textures.
        /// @param _even Texture used for even cells.
        /// @param _odd Texture used for odd cells.
        /// @param _scale Controls the size of the pattern.
        CheckerTexture(Texture* _even, Texture* _odd, double _scale);

        /// @brief Creates a checker texture using two colors.
        /// @param _even Color for even cells.
        /// @param _odd Color for odd cells.
        /// @param _scale Controls the size of the pattern.
        CheckerTexture(const Color& _even, const Color& _odd, double _scale);

        /// @brief Destructor (frees textures if needed).
        ~CheckerTexture();

        /// @brief Returns the color at given UV and position.
        ///        Decides between even/odd based on position and scale.
        Color sample(const Vec2d& uv, const Point3d& p) const override;

        /// @brief Converts UV coordinates to a 3D position (used for procedural mapping).
        Point3d uvToWorldConverter(const Vec2d& uv) const override;

        /// @brief Serializes the texture into LNT format.
        std::string serializeLNT() const override;

    private:
        Texture* even; ///< Texture/color used for even cells.
        Texture* odd;  ///< Texture/color used for odd cells.
        double scale;  ///< Controls size of checker pattern.
};

#endif