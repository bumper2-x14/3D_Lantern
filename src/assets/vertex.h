#ifndef VERTEX_H
#define VERTEX_H

#include "math/vec2.h"
#include "math/vec3.h"

/// @brief Basic vertex structure used in meshes.
///        Stores position, normal and UV coordinates.
struct Vertex{
    Vec3f position; ///< Position of the vertex in 3D space.
    Vec3f normal;   ///< Normal vector (used for lighting).
    Vec2f uv;       ///< Texture coordinates.

    /// @brief Default constructor.
    Vertex() {}

    /// @brief Creates a vertex with all attributes.
    /// @param _position Position in 3D.
    /// @param _normal Normal vector.
    /// @param _uv Texture coordinates.
    Vertex(const Vec3f& _position, const Vec3f& _normal, const Vec2f& _uv)
        : position(_position), normal(_normal), uv(_uv) {}
};

#endif