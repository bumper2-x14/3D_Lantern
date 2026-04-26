#ifndef CAMERA_H 
#define CAMERA_H

#include "math/vec3.h" 
#include "math/mat4.h"

/// @brief Represents a simple camera used to move inside the scene.
///        It stores its position, direction and projection settings.
class MD_Camera {
    public:
        float sensitivity = 0.1f; ///< Mouse sensitivity used to rotate the camera.

        /// @brief Creates a camera with default values.
        MD_Camera();

        /// @brief Creates a camera at a given position and looking direction.
        /// @param pos Initial camera position.
        /// @param front Initial direction the camera is looking at.
        MD_Camera(const Vec3f& pos, const Vec3f& front = Vec3f(0, 0, -1));

        // view / projection

        /// @brief Builds the view matrix from the current camera position and direction.
        Mat4f getViewMatrix()       const;

        /// @brief Builds the projection matrix using fov, aspect, near and far values.
        Mat4f getProjectionMatrix() const; // uses fov, aspect, near, far

        // state

        /// @brief Returns the current front direction of the camera.
        Vec3f getFront() const { return front; }

        /// @brief Returns the current up direction of the camera.
        Vec3f getUp() const { return up;    }

        /// @brief Returns the current field of view.
        float getFov() const { return fov;   }

        /// @brief Returns the current position of the camera.
        Vec3f getPosition() const { return pos; }

        /// @brief Updates the aspect ratio used by the projection matrix.
        void  setAspect(float a) { aspect = a; }

        /// @brief Updates the field of view.
        void  setFov(float f) { fov = f;    }

        /// @brief Sets the floor height used to keep the camera above the ground.
        void  setFloorY(float y) { floorY = y; }

        // per-frame update

        /// @brief Stores all inputs needed to update the camera for one frame.
        struct CameraInput {
            bool  w, s, a, d, up, down; ///< Movement keys state.
            float xoffset, yoffset; ///< Mouse movement offsets.
            float speed; ///< Movement speed for this frame.
        };

        /// @brief Updates the camera position and rotation from the current input.
        /// @param in Input values for this frame.
        void update(const CameraInput& in);

        /// @brief Runs simple tests to check the camera behavior.
        static void regressionTest(); 

    private:
        Vec3f pos   = { 0, 0,  0 }; ///< Camera position in the scene.
        Vec3f front = { 0, 0, -1 }; ///< Direction where the camera is looking.
        Vec3f up    = { 0, 1,  0 }; ///< Up direction of the camera.

        float yaw   = -90.0f; ///< Horizontal rotation angle.
        float pitch =   0.0f; ///< Vertical rotation angle.

        float fov    = 45.0f; ///< Field of view used for projection.
        float aspect = 16.0f / 9.0f; ///< Aspect ratio of the window.
        float nearZ  = 0.1f; ///< Near clipping plane.
        float farZ   = 1000.0f; ///< Far clipping plane.
        float floorY = 0.0f; ///< Minimum height allowed for the camera.
};

#endif 