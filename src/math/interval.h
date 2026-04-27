#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

/// @brief Generic interval [min, max].
///        Used for ranges (ray t, bounding boxes, etc).
template <typename T>
class Interval{
    public:
        T min, max; ///< Interval bounds.

        /// @brief Default constructor → empty interval (min > max).
        Interval(): 
            min(+std::numeric_limits<T>::infinity()),
            max(-std::numeric_limits<T>::infinity()){}

        /// @brief Creates an interval with given bounds.
        Interval(T min, T max): min(min), max(max) {}

        /// @brief Creates interval enclosing two intervals.
        Interval(const Interval<T>& a, const Interval<T>& b){
            // Create the interval tightly enclosing the two input intervals.
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }

        /// @brief Returns interval size.
        T size() const{
            return max - min;
        }

        /// @brief Checks if value is inside [min, max].
        bool contains(T x) const{
            return x >= min && x <= max;
        }

        /// @brief Checks if value is strictly inside (min, max).
        bool surrounds(T x) const{
            return x > min && x < max;
        }

        /// @brief Clamps a value into the interval.
        T clamp(T x) const{
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        /// @brief Expands interval by delta.
        Interval<T> expand(T delta) const{
            auto padding = delta / 2;
            return Interval<T>(min - padding, max + padding);
        }

        static const Interval<T> empty, universe; ///< Special intervals.
};

using Intervali = Interval<int>;
using Intervalf = Interval<float>;
using Intervald = Interval<double>;

/// @brief Empty interval definition.
template <typename T> 
const Interval<T> Interval<T>::empty = Interval(+std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity());

/// @brief Infinite interval definition.
template <typename T>
const Interval<T> Interval<T>::universe = Interval(-std::numeric_limits<T>::infinity(), +std::numeric_limits<T>::infinity());

/// @brief Shifts interval by a value.
template <typename T> 
Interval<T> operator+(const Interval<T>& ival, double displacement){
    return Interval<T>(ival.min + displacement, ival.max + displacement);
}

/// @brief Shifts interval by a value (reverse order).
template <typename T> 
Interval<T> operator+(double displacement, const Interval<T>& ival) {
    return ival + displacement;
}

#endif