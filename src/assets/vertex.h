#ifndef VERTEX_H
#define VERTEX_H

#include "../math/vec2.h"
#include "../math/vec3.h"

struct Vertex{
    Vec3 position;
    Vec3 normal;
    Vec2 uv;

    Vertex() {}

    Vertex(const Vec3& _position, const Vec3& _normal, const Vec2& _uv)
        : position(_position), normal(_normal), uv(_uv) {}
};

#endif