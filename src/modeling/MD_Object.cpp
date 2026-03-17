#include "MD_Object.h"

MD_Object::MD_Object(): shape(nullptr), transform(nullptr) {}

MD_Object::MD_Object(MD_Shape* _shape, Transform* _transform){
    shape = _shape;
    transform = _transform;
}