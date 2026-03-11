#include <iostream>
#include <cassert>

#include "math/mat4.h"

using namespace std::chrono;

int main() {

    std::cout << "======== MAT4 REGRESSION TEST ========" << std::endl;

    // 1. IDENTITY TEST
    Mat4<float> I = Mat4<float>::identity();
    Vec4f v(1,2,3,1);

    Vec4f r = I * v;

    assert(r.x == v.x);
    assert(r.y == v.y);
    assert(r.z == v.z);
    assert(r.w == v.w);

    std::cout << "Identity test passed\n";

    // 2. TRANSLATION TEST
    Vec3f t(10,20,30);
    Mat4<float> T = Mat4<float>::translation(t);
    Vec3f p(1,2,3);
    Vec3f p2 = T * p;

    assert(p2.x == 11);
    assert(p2.y == 22);
    assert(p2.z == 33);

    std::cout << "Translation test passed\n";

    // 3. SCALE TEST
    Mat4<float> S = Mat4<float>::scale(Vec3f(2,3,4));

    Vec3f s = S * Vec3f(1,1,1);

    assert(s.x == 2);
    assert(s.y == 3);
    assert(s.z == 4);

    std::cout << "Scale test passed\n";

    // 4. ROTATION TEST
    Mat4<float> R = Mat4<float>::rotationZ(M_PI * 0.5);

    Vec3f a(1,0,0);
    Vec3f b = R * a;

    assert(std::abs(b.x) < 1e-5);
    assert(std::abs(b.y - 1) < 1e-5);

    std::cout << "Rotation test passed\n";

    // 5. MATRIX MULTIPLICATION
    Mat4<float> M = T * S;

    Vec3f q = M * Vec3f(1,1,1);

    assert(q.x == 12);
    assert(q.y == 23);
    assert(q.z == 34);

    std::cout << "Matrix multiplication passed\n";

    // 6. INVERSE TEST
    Mat4<float> inv = T.inverseTRS();
    Mat4<float> prod = T * inv;

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            assert(std::abs(prod(i,j) - I(i,j)) < 1e-4);

    std::cout << "Inverse test passed\n";

    // 7. TRANSPOSE TEST
    Mat4<float> M2 = Mat4<float>(
        1,2,3,4,
        5,6,7,8,
        9,10,11,12,
        13,14,15,16
    );

    Mat4<float> Mt = M2.transpose();

    assert(Mt(0,1) == M2(1,0));
    assert(Mt(2,3) == M2(3,2));

    std::cout << "Transpose test passed\n";

    // 8. LOOKAT TEST
    Vec3f eye(0,0,5);
    Vec3f target(0,0,0);
    Vec3f up(0,1,0);

    Mat4<float> view = Mat4<float>::lookAt(eye,target,up);

    Vec3f origin = view * Vec3f(0,0,0);

    assert(std::abs(origin.z + 5) < 1e-5);

    std::cout << "LookAt test passed\n";

    // 9. SPEED TEST
    const int N = 5'000'000;

    Mat4<float> A = Mat4<float>::rotationX(0.5f);
    Mat4<float> B = Mat4<float>::rotationY(0.3f);
    Mat4<float> C;

    auto start = high_resolution_clock::now();

    for(int i=0; i<N ;i++){
        C = A * B;
    }

    auto end = high_resolution_clock::now();

    double time = duration<double>(end-start).count();

    std::cout << "\nMatrix multiplication speed test\n";
    std::cout << N << " multiplications in " << time << " seconds\n";
    std::cout << "Average: " << (time/N)*1e9 << " ns per multiply\n";

    // 10. VECTOR TRANSFORM SPEED
    Vec4f vec(1,2,3,1);

    start = high_resolution_clock::now();

    for(int i=0;i<N;i++)
    {
        vec = A * vec;
    }

    end = high_resolution_clock::now();

    time = duration<double>(end-start).count();

    std::cout << "\nVector transform speed test\n";
    std::cout << N << " transforms in " << time << " seconds\n";
    std::cout << "Average: " << (time/N)*1e9 << " ns per transform\n";

    std::cout << "\n======== ALL MAT4 TESTS PASSED ========\n";

    return 0;
}