#ifndef _UTILITY_H
#define _UTILITY_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

template <typename T> double infinity = std::numeric_limits<T>::infinity();

constexpr double PI = 3.14159265358979323846;

inline double toRadians(double degrees) { return degrees * (PI / 180.0); }
inline double toDegrees(double radians) { return radians * (180.0 / PI); }

inline float toRadians(float degrees) { return degrees * (static_cast<float>(PI) / 180.0f); }
inline float toDegrees(float radians) { return radians * (180.0f / static_cast<float>(PI)); }

inline void randomSeed(unsigned int seed) { std::srand(seed); }
inline void randomSeedTime() { std::srand(static_cast<unsigned int>(std::time(nullptr))); }


inline int randomInt(int min, int max){
    return min + std::rand() % (max - min + 1);
}

inline float randomFloat(float min, float max){
    return min + (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * (max - min);
}

inline double randomDouble(double min, double max) {
    return min + (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)) * (max - min);
}

template <typename T>
inline T randomizer(T min, T max) {
    return min + (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX)) * (max - min);
}


#endif