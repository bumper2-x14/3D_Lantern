#include <iostream>
#include <cassert>

#include "math/vec2.h"

int main() {
    // --- CONSTRUCTOR TEST ---
    Vec2<float> v1;
    assert(v1.x == 0 && v1.y == 0);

    Vec2<float> v2(3.0f, 4.0f);
    assert(v2.x == 3.0f && v2.y == 4.0f);

    // --- INDEX OPERATOR TEST ---
    assert(v2[0] == 3.0f);
    assert(v2[1] == 4.0f);

    // --- ADDITION TEST ---
    Vec2<float> v3(1.0f, 2.0f);
    Vec2<float> v4 = v2 + v3;
    assert(v4.x == 4.0f && v4.y == 6.0f);

    // --- SUBTRACTION TEST ---
    Vec2<float> v5 = v2 - v3;
    assert(v5.x == 2.0f && v5.y == 2.0f);

    // --- SCALAR MULTIPLICATION ---
    Vec2<float> v6 = v3 * 2.0f;
    assert(v6.x == 2.0f && v6.y == 4.0f);

    Vec2<float> v7 = 2.0f * v3;
    assert(v7.x == 2.0f && v7.y == 4.0f);

    // --- DIVISION ---
    Vec2<float> v8 = v7 / 2.0f;
    assert(v8.x == 1.0f && v8.y == 2.0f);

    // --- DOT PRODUCT ---
    float d = dot(v2, v3);
    assert(d == (3.0f*1.0f + 4.0f*2.0f));

    // --- LENGTH ---
    float len = v2.length();
    assert(std::abs(len - 5.0f) < 1e-6);

    // --- LENGTH SQUARED ---
    assert(v2.lengthSquared() == 25.0f);

    // --- NORMALIZATION ---
    Vec2<float> vn = normalize(v2);
    assert(std::abs(vn.length() - 1.0f) < 1e-6);

    // --- DISTANCE ---
    float dist = distance(v2, v3);
    assert(std::abs(dist - std::sqrt(8.0f)) < 1e-6);

    // --- EQUALITY ---
    Vec2<float> v9(3.0f, 4.0f);
    assert(v2 == v9);
    assert(v2 != v3);

    std::cout << "All Vec2 regression tests passed!" << std::endl;

    return 0;
}