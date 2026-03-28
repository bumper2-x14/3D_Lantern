#ifndef PERLIN_H
#define PERLIN_H

#include "math/vec3.h"
#include "math/point3.h"


class Perlin {
    public:
        Perlin();
        double noise(const Point3d& p) const;
        double turbulence(const Point3d& p, int depth) const;

    private:
        static const int cluster_count = 256;
        Vec3d rand_vec[cluster_count];
        int permuted_x[cluster_count];
        int permuted_y[cluster_count];
        int permuted_z[cluster_count];

        static void permuteArray(int* arr, int n);
        static void generatePermutedPerlin(int* arr);
        static double perlinInterpolate(const Vec3d c[2][2][2], double u, double v, double w);
};

#endif