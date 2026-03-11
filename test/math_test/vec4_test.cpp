#include <iostream>
#include <cassert>

#include "math/vec4.h"

int main() {
    // --- CONSTRUCTOR TEST ---
    Vec4<float> v1;
    assert(v1.x == 0 && v1.y == 0 && v1.z == 0 && v1.w == 0);

    Vec4<float> v2(1.0f, 2.0f, 3.0f, 4.0f);
    assert(v2.x == 1.0f && v2.y == 2.0f && v2.z == 3.0f && v2.w == 4.0f);

    // --- INDEX OPERATOR ---
    assert(v2[0] == 1.0f);
    assert(v2[1] == 2.0f);
    assert(v2[2] == 3.0f);
    assert(v2[3] == 4.0f);

    // --- ADDITION ---
    Vec4<float> v3(4.0f, 3.0f, 2.0f, 1.0f);
    Vec4<float> v4 = v2 + v3;
    assert(v4.x == 5.0f && v4.y == 5.0f && v4.z == 5.0f && v4.w == 5.0f);

    // --- SUBTRACTION ---
    Vec4<float> v5 = v2 - v3;
    assert(v5.x == -3.0f && v5.y == -1.0f && v5.z == 1.0f && v5.w == 3.0f);

    // --- SCALAR MULTIPLICATION ---
    Vec4<float> v6 = v2 * 2.0f;
    assert(v6.x == 2.0f && v6.y == 4.0f && v6.z == 6.0f && v6.w == 8.0f);

    // --- DIVISION ---
    Vec4<float> v7 = v6 / 2.0f;
    assert(v7.x == 1.0f && v7.y == 2.0f && v7.z == 3.0f && v7.w == 4.0f);

    // --- DOT PRODUCT ---
    float d = dot(v2, v3);
    assert(d == (1*4 + 2*3 + 3*2 + 4*1));

    // --- LENGTH ---
    Vec4<float> v8(0.0f, 0.0f, 3.0f, 4.0f);
    assert(std::abs(v8.length() - 5.0f) < 1e-6);

    // --- LENGTH SQUARED ---
    assert(v8.lengthSquared() == 25.0f);

    // --- NORMALIZATION ---
    Vec4<float> vn = normalize(v8);
    assert(std::abs(vn.length() - 1.0f) < 1e-6);

    // --- EQUALITY ---
    Vec4<float> v9(1.0f, 2.0f, 3.0f, 4.0f);
    assert(v2 == v9);
    assert(v2 != v3);

    std::cout << "All Vec4 regression tests passed!" << std::endl;

    return 0;
}