#ifndef _UTILITY_H
#define _UTILITY_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>

template <typename T> double infinity = std::numeric_limits<T>::infinity();

constexpr double PI = 3.14159265358979323846;

inline double toRadians(double degrees) { return degrees * (PI / 180.0); }
inline double toDegrees(double radians) { return radians * (180.0 / PI); }

inline float toRadians(float degrees) { return degrees * (static_cast<float>(PI) / 180.0f); }
inline float toDegrees(float radians) { return radians * (180.0f / static_cast<float>(PI)); }

inline void randomSeed(unsigned int seed) { std::srand(seed); }
inline void randomSeedTime() { std::srand(static_cast<unsigned int>(std::time(nullptr))); }


inline int randomInt(int min, int max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

inline float randomFloat(float min, float max){
    return min + (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * (max - min);
}

inline double randomDouble(double min, double max) {
    return min + (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)) * (max - min);
}

template <typename T>
inline T randomizer(T min, T max) {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<T> dist(T(0), T(1));
    return min + dist(gen) * (max - min);
}

inline double schlick(double cosine, double ref_index) {
    double r0 = (1 - ref_index) / (1 + ref_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}

#endif
