#include <iostream>
#include <cassert>

#include "math/vec3.h"

int main() {

    // --- CONSTRUCTOR TEST ---
    Vec3<float> v1;
    assert(v1.x == 0 && v1.y == 0 && v1.z == 0);

    Vec3<float> v2(1.0f, 2.0f, 3.0f);
    assert(v2.x == 1.0f && v2.y == 2.0f && v2.z == 3.0f);

    // --- INDEX OPERATOR TEST ---
    assert(v2[0] == 1.0f);
    assert(v2[1] == 2.0f);
    assert(v2[2] == 3.0f);

    // --- ADDITION ---
    Vec3<float> v3(4.0f, 5.0f, 6.0f);
    Vec3<float> v4 = v2 + v3;
    assert(v4.x == 5.0f && v4.y == 7.0f && v4.z == 9.0f);

    // --- SUBTRACTION ---
    Vec3<float> v5 = v3 - v2;
    assert(v5.x == 3.0f && v5.y == 3.0f && v5.z == 3.0f);

    // --- SCALAR MULTIPLICATION ---
    Vec3<float> v6 = v2 * 2.0f;
    assert(v6.x == 2.0f && v6.y == 4.0f && v6.z == 6.0f);

    Vec3<float> v7 = 2.0f * v2;
    assert(v7.x == 2.0f && v7.y == 4.0f && v7.z == 6.0f);

    // --- DIVISION ---
    Vec3<float> v8 = v7 / 2.0f;
    assert(v8.x == 1.0f && v8.y == 2.0f && v8.z == 3.0f);

    // --- DOT PRODUCT ---
    float d = dot(v2, v3);
    assert(d == (1*4 + 2*5 + 3*6));

    // --- CROSS PRODUCT ---
    Vec3<float> v9 = cross(v2, v3);
    assert(v9.x == -3.0f);
    assert(v9.y == 6.0f);
    assert(v9.z == -3.0f);

    // --- LENGTH ---
    Vec3<float> v10(0.0f, 3.0f, 4.0f);
    assert(std::abs(v10.length() - 5.0f) < 1e-6);

    // --- LENGTH SQUARED ---
    assert(v10.lengthSquared() == 25.0f);

    // --- NORMALIZATION ---
    Vec3<float> vn = normalize(v10);
    assert(std::abs(vn.length() - 1.0f) < 1e-6);

    // --- DISTANCE ---
    float dist = distance(v2, v3);
    assert(std::abs(dist - std::sqrt(27.0f)) < 1e-6);

    // --- EQUALITY ---
    Vec3<float> v11(1.0f, 2.0f, 3.0f);
    assert(v2 == v11);
    assert(v2 != v3);

    std::cout << "All Vec3 regression tests passed!" << std::endl;

    return 0;
}