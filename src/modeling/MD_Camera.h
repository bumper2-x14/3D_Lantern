#ifndef CAMERA_H 
#define CAMERA_H

#include "math/vec3.h" 
#include "math/mat4.h"

class MD_Camera {
    public:
        float sensitivity = 0.1f;

        MD_Camera();
        MD_Camera(const Vec3f& pos, const Vec3f& front = Vec3f(0, 0, -1));

        // view / projection
        Mat4f getViewMatrix()       const;
        Mat4f getProjectionMatrix() const; // uses fov, aspect, near, far

        // state
        Vec3f getFront() const { return front; }
        Vec3f getUp() const { return up;    }
        float getFov() const { return fov;   }
        Vec3f getPosition() const { return pos; }
        void  setAspect(float a) { aspect = a; }
        void  setFov(float f) { fov = f;    }
        void  setFloorY(float y) { floorY = y; }

        // per-frame update
        struct CameraInput {
            bool  w, s, a, d, up, down;
            float xoffset, yoffset;
            float speed;
        };
        void update(const CameraInput& in);

        static void regressionTest(); 

    private:
        Vec3f pos   = { 0, 0,  0 };
        Vec3f front = { 0, 0, -1 };
        Vec3f up    = { 0, 1,  0 };

        float yaw   = -90.0f;
        float pitch =   0.0f;

        float fov    = 45.0f;
        float aspect = 16.0f / 9.0f;
        float nearZ  = 0.1f;
        float farZ   = 1000.0f;
        float floorY = 0.0f;
};

#endif