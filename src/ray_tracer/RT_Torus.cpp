#include <cmath>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "RT_Torus.h"

// tiny local polynomial solvers (no external deps)

static int solveCubic(double a, double b, double c, double d, double roots[3]);
static int solveQuadratic(double a, double b, double c, double roots[2]);

// Quadratic: a*t^2 + b*t + c = 0
static int solveQuadratic(double a, double b, double c, double roots[2]) {
    double disc = b*b - 4.0*a*c;
    if (disc < 0.0) return 0;
    double sq = std::sqrt(disc);
    roots[0] = (-b - sq) / (2.0*a);
    roots[1] = (-b + sq) / (2.0*a);
    return (disc == 0.0) ? 1 : 2;
}

// Depressed cubic: t^3 + p*t + q = 0  (Cardano / trig method)
static int solveCubicDepressed(double p, double q, double roots[3]) {
    double disc = -4.0*p*p*p - 27.0*q*q;
    if (disc >= 0.0) {
        // Three real roots — trigonometric method
        double m = 2.0 * std::sqrt(-p / 3.0);
        double theta = std::acos(3.0*q / (p * m)) / 3.0;
        constexpr double TAU3 = 2.0 * M_PI / 3.0;
        roots[0] = m * std::cos(theta);
        roots[1] = m * std::cos(theta - TAU3);
        roots[2] = m * std::cos(theta - 2.0*TAU3);
        return 3;
    } else {
        // One real root — Cardano
        double sq = std::sqrt(-disc / 108.0);
        double cbrtA = std::cbrt(-q/2.0 + sq);
        double cbrtB = std::cbrt(-q/2.0 - sq);
        roots[0] = cbrtA + cbrtB;
        return 1;
    }
}

// General cubic: a*t^3 + b*t^2 + c*t + d = 0
static int solveCubic(double a, double b, double c, double d, double roots[3]) {
    if (std::abs(a) < 1e-12) {
        double qr[2];
        int n = solveQuadratic(b, c, d, qr);
        for (int i = 0; i < n; ++i) roots[i] = qr[i];
        return n;
    }
    // Depress: substitute t = u - b/(3a)
    double inv_a = 1.0 / a;
    double B = b * inv_a, C = c * inv_a, D = d * inv_a;
    double p = C - B*B / 3.0;
    double q = D - B*C/3.0 + 2.0*B*B*B / 27.0;
    int n = solveCubicDepressed(p, q, roots);
    double shift = B / 3.0;
    for (int i = 0; i < n; ++i) roots[i] -= shift;
    return n;
}

// General quartic: c4*t^4 + c3*t^3 + c2*t^2 + c1*t + c0 = 0  (Ferrari's method)
int RT_Torus::solveQuartic(double c4, double c3, double c2,
                            double c1, double c0, double out[4]) {
    if (std::abs(c4) < 1e-12)
        return solveCubic(c3, c2, c1, c0, reinterpret_cast<double(*)[3]>(out)[0]);

    // Normalise to monic: t^4 + b*t^3 + c*t^2 + d*t + e
    double inv = 1.0 / c4;
    double b = c3*inv, c = c2*inv, d = c1*inv, e = c0*inv;

    // Depress: t = u - b/4
    double s = b / 4.0;
    double p = c - 6.0*s*s;
    double q = d + 8.0*s*s*s - 2.0*s*c;   // actually should be: d - 2*s*c + 8*s^3? let me recompute properly
    // Ferrari: resolvent cubic in m
    // p = c - 6s^2
    // q = 2s^3 - sc + d/2  (wait — let's redo from scratch cleanly)
    // Recompute using standard substitution:
    //   t = u - b/4
    //   p = c - 3b^2/8
    //   q = b^3/8 - bc/2 + d
    //   r_const = -3b^4/256 + cb^2/16 - bd/4 + e
    double B = b, C = c, D = d, E = e;
    p = C - 3.0*B*B/8.0;
    q = B*B*B/8.0 - B*C/2.0 + D;
    double r_const = -3.0*B*B*B*B/256.0 + C*B*B/16.0 - B*D/4.0 + E;

    int nRoots = 0;

    if (std::abs(q) < 1e-12) {
        // Biquadratic: u^4 + p*u^2 + r_const = 0
        double qr[2];
        int n = solveQuadratic(1.0, p, r_const, qr);
        for (int i = 0; i < n; ++i) {
            if (qr[i] >= 0.0) {
                double sq = std::sqrt(qr[i]);
                out[nRoots++] = -sq - s;
                out[nRoots++] =  sq - s;
            }
        }
    } else {
        // Resolvent cubic: 8m^3 + 8p*m^2 + (2p^2-8r)*m - q^2 = 0
        double cubRoots[3];
        int nC = solveCubic(8.0, 8.0*p, 2.0*p*p - 8.0*r_const, -q*q, cubRoots);
        // Pick the largest real root to maximise numerical stability
        double m = cubRoots[0];
        for (int i = 1; i < nC; ++i) m = std::max(m, cubRoots[i]);

        double sqm = (m > 0.0) ? std::sqrt(m) : 0.0;
        double A1 = sqm, A2 = -sqm;
        double B1 = p/2.0 + m - (sqm > 1e-12 ? q/(4.0*sqm) : 0.0);
        double B2 = p/2.0 + m + (sqm > 1e-12 ? q/(4.0*sqm) : 0.0);

        double qr[2];
        if (solveQuadratic(1.0, 2.0*A1, 2.0*B1, qr) == 2) {
            out[nRoots++] = qr[0] - s;
            out[nRoots++] = qr[1] - s;
        }
        if (solveQuadratic(1.0, 2.0*A2, 2.0*B2, qr) == 2) {
            out[nRoots++] = qr[0] - s;
            out[nRoots++] = qr[1] - s;
        }
    }
    return nRoots;
}

// ── RT_Torus ──────────────────────────────────────────────────────────────────

RT_Torus::RT_Torus(RT_Material* mat) : material(mat) {}

void RT_Torus::setTransform(const TRSTransformd& t) {
    RT_Object::setTransform(t);
    // Tight AABB in local space then transformed
    double ext_xy = R + r;
    setBoundingBox(computeBBox(
        BoundingBoxd(Point3d(-ext_xy, -r, -ext_xy),
                     Point3d( ext_xy,  r,  ext_xy))));
}

bool RT_Torus::rayIntersect(const Rayd& ray, const Intervald& t_interval,
                             RT_Record& rec) const {
    Mat4d inv = getInverse();
    Rayd  lr  = transformRay(ray, inv);   // ray in local (torus) space

    const Vec3d& d = lr.getDirection();
    const Vec3d  o = (Vec3d)lr.getOrigin();  // treat origin as vec for dot products

    // Build quartic coefficients for:
    //   (|P|^2 + R^2 - r^2)^2 = 4*R^2*(x^2 + z^2)
    // where P = o + t*d, and torus hole-axis is Y.
    //
    // Let S = |d|^2, T = dot(o,d), U = |o|^2 + R^2 - r^2, V = R^2
    double S  = d.lengthSquared();
    double T  = dot(o, d);
    double U  = o.lengthSquared() + R*R - r*r;
    double V  = 4.0 * R*R; // coefficient multiplier
    // xz dot products (Y excluded)
    double dxz2 = d.x*d.x + d.z*d.z;
    double oxzd  = o.x*d.x + o.z*d.z;
    double oxz2  = o.x*o.x + o.z*o.z;

    // Quartic: (S*t^2 + 2T*t + U)^2 - V*(dxz2*t^2 + 2*oxzd*t + oxz2) = 0
    double co4 = S*S;
    double co3 = 4.0*S*T;
    double co2 = 2.0*S*U + 4.0*T*T - V*dxz2;
    double co1 = 4.0*T*U - V*2.0*oxzd;
    double co0 = U*U - V*oxz2;

    double roots[4];
    int n = solveQuartic(co4, co3, co2, co1, co0, roots);
    if (n == 0) return false;

    // Pick the smallest root inside t_interval
    double best = std::numeric_limits<double>::infinity();
    for (int i = 0; i < n; ++i)
        if (t_interval.surrounds(roots[i]) && roots[i] < best)
            best = roots[i];

    if (std::isinf(best)) return false;

    // Hit point in local space
    Point3d lp = lr.at(best);

    // Local normal: gradient of the implicit function
    //   F(x,y,z) = (x^2+y^2+z^2+R^2-r^2)^2 - 4R^2(x^2+z^2)
    double k = lp.x*lp.x + lp.y*lp.y + lp.z*lp.z + R*R - r*r;
    Vec3d local_n(
        4.0*lp.x*(k - 2.0*R*R),
        4.0*lp.y* k,
        4.0*lp.z*(k - 2.0*R*R)
    );
    // (no need to normalise before transformNormal — it normalises internally)

    // UV mapping: phi around hole axis (Y), theta around tube
    double phi = std::atan2(lp.z, lp.x);  // [-π, π]
    Point3d ring(R * std::cos(phi), 0.0, R * std::sin(phi));
    Vec3d toP = (Vec3d)lp - (Vec3d)ring;
    double theta = std::atan2(toP.y, std::sqrt(toP.x*toP.x + toP.z*toP.z) - R);

    rec.uv.x = (phi + M_PI) / (2.0 * M_PI);  // u ∈ [0,1]
    rec.uv.y = (theta + M_PI) / (2.0 * M_PI);  // v ∈ [0,1]
    rec.t = best;
    rec.p = getMatrix() * lp;
    Vec3d world_n = transformNormal(local_n, inv);
    rec.setNormal(ray, world_n);
    rec.material = material;
    return true;
}
