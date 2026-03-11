#include <iostream>
#include <cassert>

#include "math/interval.h"

int main() {

    std::cout << "======== INTERVAL REGRESSION TEST ========\n";

    // DEFAULT EMPTY INTERVAL
    Interval<float> e;
    assert(e.min > e.max);

    std::cout << "Default empty interval test passed\n";

    // BASIC CONSTRUCTOR
    Interval<float> a(1.0f, 5.0f);

    assert(a.min == 1.0f);
    assert(a.max == 5.0f);
    assert(a.size() == 4.0f);

    std::cout << "Constructor and size test passed\n";

    // CONTAINS
    assert(a.contains(1.0f));
    assert(a.contains(3.0f));
    assert(a.contains(5.0f));
    assert(!a.contains(6.0f));

    std::cout << "Contains test passed\n";

    // SURROUNDS
    assert(a.surrounds(3.0f));
    assert(!a.surrounds(1.0f));
    assert(!a.surrounds(5.0f));

    std::cout << "Surrounds test passed\n";

    // CLAMP
    assert(a.clamp(0.0f) == 1.0f);
    assert(a.clamp(6.0f) == 5.0f);
    assert(a.clamp(3.0f) == 3.0f);

    std::cout << "Clamp test passed\n";

    // EXPAND
    Interval<float> b = a.expand(2.0f);

    assert(b.min == 0.0f);
    assert(b.max == 6.0f);

    std::cout << "Expand test passed\n";

    // UNION OF INTERVALS
    Interval<float> c(4.0f, 10.0f);
    Interval<float> u(a, c);

    assert(u.min == 1.0f);
    assert(u.max == 10.0f);

    std::cout << "Union constructor test passed\n";

    // STATIC INTERVALS
    assert(Interval<float>::universe.contains(1e30f));
    assert(!Interval<float>::empty.contains(0.0f));

    std::cout << "Static intervals test passed\n";

    // INTERVAL DISPLACEMENT
    Interval<float> d = a + 2.0;

    assert(d.min == 3.0f);
    assert(d.max == 7.0f);

    Interval<float> d2 = 2.0 + a;

    assert(d2.min == 3.0f);
    assert(d2.max == 7.0f);

    std::cout << "Displacement operator test passed\n";
    std::cout << "\n======== ALL INTERVAL TESTS PASSED ========\n";

    return 0;
}