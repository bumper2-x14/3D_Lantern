#include <iostream>
#include <cassert>

#include "math/transform.h"

using namespace std::chrono;


int main() {

    std::cout << "======== TRANSFORM REGRESSION TEST ========\n";

    // TRANSLATION TEST
    Transform T = Transform::translate(Vec3f(10,20,30));

    Vec3f p(1,2,3);
    Vec3f p2 = T * p;

    assert(p2.x == 11);
    assert(p2.y == 22);
    assert(p2.z == 33);

    std::cout << "Translation test passed\n";


    // SCALE TEST
    Transform S = Transform::scale(Vec3f(2,3,4));

    Vec3f s = S * Vec3f(1,1,1);

    assert(s.x == 2);
    assert(s.y == 3);
    assert(s.z == 4);

    std::cout << "Scale test passed\n";

    // ROTATION TEST
    Transform R = Transform::rotateZ(M_PI * 0.5f);

    Vec3f a(1,0,0);
    Vec3f b = R * a;

    assert(std::abs(b.x) < 1e-5);
    assert(std::abs(b.y - 1) < 1e-5);

    std::cout << "Rotation test passed\n";

    // COMPOSITION TEST
    Transform C = T * S;

    Vec3f q = C * Vec3f(1,1,1);

    assert(q.x == 12);
    assert(q.y == 23);
    assert(q.z == 34);

    std::cout << "Composition test passed\n";


    // INVERSE TEST
    Transform Ti = T.inverse();

    Vec3f back = Ti * (T * p);

    assert(std::abs(back.x - p.x) < 1e-5);
    assert(std::abs(back.y - p.y) < 1e-5);
    assert(std::abs(back.z - p.z) < 1e-5);

    std::cout << "Inverse test passed\n";


    // LOOKAT TEST
    Vec3f eye(0,0,5);
    Vec3f target(0,0,0);
    Vec3f up(0,1,0);

    Transform cam = Transform::lookAt(eye,target,up);

    Vec3f origin = cam.inverse() * Vec3f(0,0,0);

    assert(std::abs(origin.z + 5) < 1e-5);

    std::cout << "LookAt test passed\n";


    // PERFORMANCE TEST
    const int N = 5'000'000;

    Transform A = Transform::rotateX(0.5f);
    Transform B = Transform::translate(Vec3f(1,2,3));
    Transform Cperf;

    auto start = high_resolution_clock::now();

    for(int i=0;i<N;i++)
    {
        Cperf = A * B;
    }

    auto end = high_resolution_clock::now();

    double time = duration<double>(end-start).count();

    std::cout << "\nTransform composition speed test\n";
    std::cout << N << " compositions in " << time << " seconds\n";
    std::cout << "Average: " << (time/N)*1e9 << " ns per composition\n";

    // VECTOR TRANSFORM SPEED
    Vec3f v(1,2,3);

    start = high_resolution_clock::now();

    for(int i=0;i<N;i++)
    {
        v = A * v;
    }

    end = high_resolution_clock::now();

    time = duration<double>(end-start).count();

    std::cout << "\nVector transform speed test\n";
    std::cout << N << " transforms in " << time << " seconds\n";
    std::cout << "Average: " << (time/N)*1e9 << " ns per transform\n";

    std::cout << "\n======== ALL TRANSFORM TESTS PASSED ========\n";

    return 0;
}