#include <iostream>
#include <cassert>

#include "math/point3.h"

int main() {
    std::cout << "======== POINT3 REGRESSION TEST ========\n";
    // DEFAULT CONSTRUCTOR
    Point3<float> p0;
    assert(p0.x == 0 && p0.y == 0 && p0.z == 0);

    std::cout << "Default constructor test passed\n";

    // BASIC CONSTRUCTOR
    Point3<float> p1(1,2,3);

    assert(p1.x == 1);
    assert(p1.y == 2);
    assert(p1.z == 3);

    std::cout << "Constructor test passed\n";

    // INDEX OPERATOR
    assert(p1[0] == 1);
    assert(p1[1] == 2);
    assert(p1[2] == 3);

    std::cout << "Index operator test passed\n";

    // POINT + VECTOR
    Vec3f v(4,5,6);

    Point3<float> p2 = p1 + v;

    assert(p2.x == 5);
    assert(p2.y == 7);
    assert(p2.z == 9);

    std::cout << "Point + Vector test passed\n";

    // POINT - VECTOR
    Point3<float> p3 = p2 - v;

    assert(p3 == p1);

    std::cout << "Point - Vector test passed\n";

    // POINT - POINT
    Vec3f diff = p2 - p1;

    assert(diff.x == 4);
    assert(diff.y == 5);
    assert(diff.z == 6);

    std::cout << "Point - Point test passed\n";

    // COMPARISON
    Point3<float> p4(1,2,3);

    assert(p1 == p4);
    assert(p1 != p2);

    std::cout << "Comparison test passed\n";

    // COMPOUND OPERATORS
    Point3<float> p5(1,1,1);

    p5 += Vec3f(1,2,3);

    assert(p5.x == 2);
    assert(p5.y == 3);
    assert(p5.z == 4);

    p5 -= Vec3f(1,1,1);

    assert(p5.x == 1);
    assert(p5.y == 2);
    assert(p5.z == 3);

    std::cout << "Compound operator test passed\n";

    std::cout << "\n======== ALL POINT3 TESTS PASSED ========\n";
    return 0;
}