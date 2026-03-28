#include "perlin.h"
#include "math/utility.h"

Perlin::Perlin() {
    for (int i = 0; i < cluster_count; i++) {
        rand_vec[i] = normalize(Vec3d::random(0.0, 1.0));
    }

    generatePermutedPerlin(permuted_x);
    generatePermutedPerlin(permuted_y);
    generatePermutedPerlin(permuted_z);
}


double Perlin::noise(const Point3d& p) const {
    double u = p.x - std::floor(p.x);
    double v = p.y - std::floor(p.y);
    double w = p.z - std::floor(p.z);

    int i = int(std::floor(p.x));
    int j = int(std::floor(p.y));
    int k = int(std::floor(p.z));
    Vec3d c[2][2][2];

    for (int di = 0; di < 2; di++){
        for (int dj = 0; dj < 2; dj++){
            for (int dk = 0; dk < 2; dk++){
                c[di][dj][dk] =  rand_vec[
                    permuted_x[(i + di) & 255] ^
                    permuted_y[(j + dj) & 255] ^
                    permuted_z[(k + dk) & 255]
                ];  
            }
        }
    }

    return perlinInterpolate(c, u, v, w);
}

double Perlin::turbulence(const Point3d& p, int depth) const {
    double accum = 0.0;
    Point3d temp_p = p;
    double weight = 1.0;

    for (int i = 0; i < depth; i++){
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p = temp_p * 2.0;
    }

    return std::fabs(accum);
}


void Perlin::permuteArray(int* arr, int n) {
    for (int i = n-1; i > 0; i--) {
        int target = randomInt(0, i);
        int tmp = arr[i];
        arr[i] = arr[target];
        arr[target] = tmp;
    }
}

void Perlin::generatePermutedPerlin(int* arr){
    for (int i = 0; i < cluster_count; i++){
        arr[i] = i;
    }
    permuteArray(arr, cluster_count);
}


double Perlin::perlinInterpolate(const Vec3d c[2][2][2], double u, double v, double w) {
    // Hermite cubic to round off the interpolation
    double uu = u*u*(3-2*u);
    double vv = v*v*(3-2*v);
    double ww = w*w*(3-2*w);

    double accum = 0.0;

    for (int i=0; i < 2; i++){
        for (int j=0; j < 2; j++){
            for (int k=0; k < 2; k++) {
                Vec3d weight_v(u-i, v-j, w-k);
                accum += (i*uu + (1-i)*(1-uu))
                    * (j*vv + (1-j)*(1-vv))
                    * (k*ww + (1-k)*(1-ww))
                    * dot(c[i][j][k], weight_v);
            }
        }
    }

    return accum;
}