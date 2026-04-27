#ifndef PERLIN_H
#define PERLIN_H

#include "math/vec3.h"
#include "math/point3.h"

/// @brief Basic Perlin noise generator.
///        Used to create smooth procedural patterns.
class Perlin {
    public:
        /// @brief Constructor (initializes random vectors and permutations).
        Perlin();

        /// @brief Returns noise value at a given 3D point.
        double noise(const Point3d& p) const;

        /// @brief Returns turbulence (layered noise for more detail).
        /// @param p Input point.
        /// @param depth Number of layers (more = more detail).
        double turbulence(const Point3d& p, int depth) const;

    private:
        static const int cluster_count = 256; ///< Size of permutation tables.

        Vec3d rand_vec[cluster_count]; ///< Random gradient vectors.
        int permuted_x[cluster_count]; ///< Permutation table for x.
        int permuted_y[cluster_count]; ///< Permutation table for y.
        int permuted_z[cluster_count]; ///< Permutation table for z.

        /// @brief Shuffles an array (used to build permutation tables).
        static void permuteArray(int* arr, int n);

        /// @brief Generates permutation array for Perlin noise.
        static void generatePermutedPerlin(int* arr);

        /// @brief Interpolates noise values inside a cube.
        static double perlinInterpolate(const Vec3d c[2][2][2], double u, double v, double w);
};

#endif