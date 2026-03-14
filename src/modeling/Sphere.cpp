#include <vector>
#include <cmath>

#include "Sphere.h"
#include "assets/vertex.h"

Sphere::Sphere(){
    float div = 25.0;
    float delta1 = M_PI /(2.0 * div);
    float delta2 = (2 * M_PI) / div;

    for (float alpha = -M_PI /2 ; alpha < M_PI/2.0 ; alpha += delta1){
        float alpha2 = alpha + delta1;

        for (float beta=0 ; beta < 2.0 * M_PI ; beta += delta2){
            float beta2 = beta + delta2;
           
            Vec3f v0(cos(alpha2) * cos(beta),  sin(alpha2),  cos(alpha2) * sin(beta));
            Vec3f v1(cos(alpha)  * cos(beta),  sin(alpha),   cos(alpha)  * sin(beta));
            Vec3f v2(cos(alpha2) * cos(beta2), sin(alpha2),  cos(alpha2) * sin(beta2));
            Vec3f v3(cos(alpha)  * cos(beta2), sin(alpha),   cos(alpha)  * sin(beta2));
        }
    }
}
