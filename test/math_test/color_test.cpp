#include <iostream>
#include <cassert>

#include "math/color.h"


int main() {
    
    std::cout << "======== COLOR REGRESSION TEST ========\n";

    // DEFAULT CONSTRUCTOR
    Color c0;
    assert(c0.r == 0 && c0.g == 0 && c0.b == 0);

    std::cout << "Default constructor passed\n";

    // BASIC CONSTRUCTOR
    Color c1(0.2f, 0.4f, 0.6f);

    assert(c1.r == 0.2f);
    assert(c1.g == 0.4f);
    assert(c1.b == 0.6f);

    std::cout << "Constructor test passed\n";

    // CONSTRUCTOR FROM VEC3
    Vec3f v(0.1f, 0.2f, 0.3f);
    Color c2(v);

    assert(c2.r == 0.1f);
    assert(c2.g == 0.2f);
    assert(c2.b == 0.3f);

    std::cout << "Vec3 constructor test passed\n";

    // COLOR ADDITION
    Color a(0.2f,0.3f,0.4f);
    Color b(0.1f,0.2f,0.3f);

    Color sum = a + b;

    assert(std::abs(sum.r - 0.3f) < 1e-6);
    assert(std::abs(sum.g - 0.5f) < 1e-6);
    assert(std::abs(sum.b - 0.7f) < 1e-6);

    std::cout << "Addition test passed\n";

    // COLOR MULTIPLICATION (COMPONENT)
    Color mul = a * b;

    assert(std::abs(mul.r - 0.02f) < 1e-6);
    assert(std::abs(mul.g - 0.06f) < 1e-6);
    assert(std::abs(mul.b - 0.12f) < 1e-6);

    std::cout << "Component multiplication test passed\n";

    // SCALAR MULTIPLICATION
    Color scaled = a * 2.0f;

    assert(std::abs(scaled.r - 0.4f) < 1e-6);
    assert(std::abs(scaled.g - 0.6f) < 1e-6);
    assert(std::abs(scaled.b - 0.8f) < 1e-6);

    std::cout << "Scalar multiplication test passed\n";

    // SCALAR DIVISION
    Color div = scaled / 2.0f;

    assert(std::abs(div.r - 0.2f) < 1e-6);
    assert(std::abs(div.g - 0.3f) < 1e-6);
    assert(std::abs(div.b - 0.4f) < 1e-6);

    std::cout << "Scalar division test passed\n";

    // LERP
    Color start(0,0,0);
    Color end(1,1,1);

    Color mid = Color::lerp(start, end, 0.5f);

    assert(std::abs(mid.r - 0.5f) < 1e-6);
    assert(std::abs(mid.g - 0.5f) < 1e-6);
    assert(std::abs(mid.b - 0.5f) < 1e-6);

    std::cout << "Lerp test passed\n";

    // CLAMP
    Color over(1.5f, -0.5f, 0.5f);

    Color clamped = over.clamp();

    assert(clamped.r == 1.0f);
    assert(clamped.g == 0.0f);
    assert(clamped.b == 0.5f);

    std::cout << "Clamp test passed\n";

    // GAMMA CONVERSION
    Color lin(0.25f, 0.25f, 0.25f);

    Color gamma = lin.toGamma(2.0f);

    assert(std::abs(gamma.r - std::sqrt(0.25f)) < 1e-6);

    std::cout << "Gamma conversion test passed\n";

    // RGB CONVERSION
    Color white(1,1,1);
    RGB rgb = white.toRGB();

    assert(rgb.r == 255);
    assert(rgb.g == 255);
    assert(rgb.b == 255);

    std::cout << "RGB conversion test passed\n";
    std::cout << "\n======== ALL COLOR TESTS PASSED ========\n";

    return 0;
}