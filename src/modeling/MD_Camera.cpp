#include "MD_Camera.h"
#include <cmath>

MD_Camera::MD_Camera() {}

MD_Camera::MD_Camera(const Vec3f& _pos, const Vec3f& _front)
    : pos(_pos), front(normalize(_front)) {}

Mat4f MD_Camera::getViewMatrix() const {
    return Mat4f::lookAt(pos, pos + front, up);
}

Mat4f MD_Camera::getProjectionMatrix() const {
    return Mat4f::perspective(toRadians(fov), aspect, nearZ, farZ);
}

void MD_Camera::update(const CameraInput& in) {
    // horizontal forward (XZ only) so W/S doesn't change height
    Vec3f hFront = normalize(Vec3f(front.x, 0.0f, front.z));
    Vec3f right  = normalize(cross(front, up));

    if (in.w)    pos += in.speed * hFront;
    if (in.s)    pos -= in.speed * hFront;
    if (in.d)    pos += in.speed * right;
    if (in.a)    pos -= in.speed * right;
    if (in.up)   pos += in.speed * up;
    if (in.down) pos -= in.speed * up;

    if (pos.y < floorY) pos.y = floorY;

    // look
    yaw   += in.xoffset * sensitivity;
    pitch += in.yoffset * sensitivity;
    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    front = normalize(Vec3f(
        std::cos(toRadians(yaw)) * std::cos(toRadians(pitch)),
        std::sin(toRadians(pitch)),
        std::sin(toRadians(yaw)) * std::cos(toRadians(pitch))
    ));

}

void MD_Camera::regressionTest() {}
