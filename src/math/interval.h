#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

template <typename T>
class Interval{
    public:
        T min, max;

        // Defult interval is empty: min > max
        Interval(): 
            min(+std::numeric_limits<T>::infinity()),
            max(-std::numeric_limits<T>::infinity()){}

        Interval(T min, T max): min(min), max(max) {}

        Interval(const Interval<T>& a, const Interval<T>& b){
            // Create the interval tightly enclosing the two input intervals.
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }

        T size() const{
            return max - min;
        }

        bool contains(T x) const{
            return x >= min && x <= max;
        }

        bool surrounds(T x) const{
            return x > min && x < max;
        }

        T clamp(T x) const{
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        Interval<T> expand(T delta) const{
            auto padding = delta / 2;
            return Interval<T>(min - padding, max + padding);
        }

        static const Interval<T> empty, universe;
};

using Intervali = Interval<int>;
using Intervalf = Interval<float>;
using Intervald = Interval<double>;


template <typename T> 
const Interval<T> Interval<T>::empty = Interval(+std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
template <typename T>
const Interval<T> Interval<T>::universe = Interval(-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());

template <typename T> 
Interval<T> operator+(const Interval<T>& ival, double displacement){
    return Interval<T>(ival.min + displacement, ival.max + displacement);
}

template <typename T> 
Interval<T> operator+(double displacement, const Interval<T>& ival) {
    return ival + displacement;
}

#endif