#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include "math/point3.h"
#include "math/vec3.h"
#include "math/color.h"
#include "math/ray.h"

class RT_Camera {
    public:
        RT_Camera();
        RT_Camera(const Point3d& _center, const Point3d& _lookAt, const Vec3d& _view_up, 
                    double _vertical_fov, double _defocus_angle, double _focus_distance);

        RT_Camera(const Vec3d& md_pos, const Vec3d& md_lookAt,
          const Vec3d& md_up, double _vfov, double _defocus_angle,  double _focus_distance);

        void initialize(float aspect_ratio, int width, int height, int samples_per_pix);
        Rayd generateRay(int i, int j, int inner_i, int inner_j) const;

        static void regressionTest();

    private:
        Point3d center = Point3d(0,0,0); ///< Camera position in the scene
        Point3d lookAt = Point3d(0, 0, -1); ///< Point where camera is looking
        Vec3d view_up = Vec3d(0, 1, 0); ///< Up direction relative to camera
        double vertical_fov = 90; ///< Vertical focal point of view
        double defocus_angle = 0; ///< Variation angle of rays going through a pixel
        double focus_distance = 10; ///< Distance from camera's center to the perfect focus plane
        Vec3d u, v, w; ///< Vectors representing the camera orthonormal basis, u: right - v: up, w: opp the view direction
        int sqrt_spp; ///< Square root of number of samples per pixel
        double recip_sqrt_spp; ///< 1 / sqrt_spp  
        
        // Data used for going throght the pixels from viewport
        Point3d pixel00; ///< Location of 0, 0
        Vec3d u_delta; ///< Offset to pixel right
        Vec3d v_delta; ///< Offset to pixel below

        // Data related to defocus disk 
        Vec3d defocus_disk_u; ///< Defocus disk horizontal radius
        Vec3d defocus_disk_v; ///< Defocus disk vertical radius


        Vec3d sampleStartifiedPixel(int inner_i, int inner_j) const;
        Vec3d samplePixel(int i, int j) const;
        Point3d defocusDiskSample() const; 

};

#endif