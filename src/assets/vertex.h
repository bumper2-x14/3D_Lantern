#ifndef VERTEX_H
#define VERTEX_H

#include "../math/vec2.h"
#include "../math/vec3.h"

struct Vertex{
    Vec3f position;
    Vec3f normal;
    Vec2f uv;

    Vertex() {}

    Vertex(const Vec3f& _position, const Vec3f& _normal, const Vec2f& _uv)
        : position(_position), normal(_normal), uv(_uv) {}
};

#endif