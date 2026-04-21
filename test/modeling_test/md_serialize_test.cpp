#include "modeling/MD_Object.h"
#include "modeling/MD_Sphere.h"


int main() {
    std::cout << "MD_Object regression test..." << std::endl;

    TRSDataf trs;
    trs.translation = Vec3f(1.0f, 2.0f, 3.0f);
    trs.rotation    = Vec3f(10.0f, 20.0f, 30.0f);
    trs.scale       = Vec3f(1.0f, 2.0f, 1.5f);

    MD_Sphere sphere(10, 10);
    MD_Object obj("test_object", &sphere, nullptr, trs);

    std::string result = obj.serialize();

    std::cout<<result<<std::endl;

    std::cout << "MD_Object serialize test passed." << std::endl;
} 