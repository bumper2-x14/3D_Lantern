#include <cmath>
#include <cassert>

#include "math/utility.h"
#include "RT_Camera.h"

RT_Camera::RT_Camera() {}

RT_Camera::RT_Camera(const Point3d& _center, const Point3d& _lookAt, const Vec3d& _view_up, 
                        double _vertical_fov, double _defocus_angle, double _focus_distance):
        center(_center), lookAt(_lookAt), view_up(_view_up), vertical_fov(_vertical_fov), 
        defocus_angle(_defocus_angle), focus_distance(_focus_distance) {}


void RT_Camera::initialize(float aspect_ratio, int width, int height, int samples_per_pix) {
    // Stratified sampling: divide pixel into sqrt_spp × sqrt_spp sub-cells
    sqrt_spp = int(std::sqrt(samples_per_pix));
    recip_sqrt_spp = 1.0 / sqrt_spp; 

    // Weight for averaging accumulated sample colors
    // double sample_scale = 1.0 / samples_per_pix; //// TODO: Will be moved to RT_Renderer
    
    // Calculate camera basis vectors
    w = normalize(center - lookAt);
    u = normalize(cross(view_up, w));
    v = cross(w, u);

    // Calculate viewport dimentions
    double theta = toRadians(vertical_fov);
    double h = std::tan(theta/2);
    double viewport_height = 2 * h * focus_distance;
    double viewport_width = viewport_height * (double(width) / height);

    // Calculate the vectors across viewport u(horizontal) and v(vertical)
    // Note: RT_Camera uses right-hand coordinate system
    Vec3d viewport_u = u * viewport_width;
    Vec3d viewport_v = -v * viewport_height; // Down viewport vertical edge

    u_delta = viewport_u / width;
    v_delta = viewport_v / height;

    Point3d upper_left_pix = center - (focus_distance * w) - (viewport_u / 2) - (viewport_v / 2);
    pixel00 = upper_left_pix + (0.5 * (u_delta + v_delta));

    // Calculate the camera defocus disk basis vectors
    double defocus_radius = focus_distance * std::tan(toRadians(defocus_angle / 2));
    defocus_disk_u = defocus_radius * u;
    defocus_disk_v = defocus_radius * v;
}

Vec3d RT_Camera::samplePixel(int i, int j) const {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square
    return Vec3d(randomDouble(-0.5, 0.5), randomDouble(-0.5, 0.5), 0);
}

Vec3d RT_Camera::sampleStartifiedPixel(int inner_i, int inner_j) const {
    // Return the vector to a random place in a sub_pixel in the [-.5,-.5]-[+.5,+.5] unit square
    double sample_x = ((inner_i + randomDouble(0, 1)) * recip_sqrt_spp) - 0.5;
    double sample_y = ((inner_j + randomDouble(0, 1)) * recip_sqrt_spp) - 0.5; 
    
    return Vec3d(sample_x, sample_y, 0);
}

Point3d RT_Camera::defocusDiskSample() const {
    // Returns a random point in the camera defocus disk.
    Vec3d sample = UnitDiskRandom<double>();
    return center + (sample.x * defocus_disk_u) + (sample.y * defocus_disk_v);
}

Rayd RT_Camera::generateRay(int i, int j, int inner_i, int inner_j) const {
    // Generate a ray from a random point on the defocus disk of camera in the direction of a randomly sampled 
    // point around the i, j location of image with startified sampleing

    Vec3d offset_samp = sampleStartifiedPixel(inner_i, inner_j);
    Point3d pixel_samp = pixel00 + ((i + offset_samp.x) * u_delta) + ((j + offset_samp.y) * v_delta);

    Point3d ray_orig = (defocus_angle <= 0) ? center : defocusDiskSample();
    Vec3d ray_dir = pixel_samp - ray_orig;
    double ray_time = randomDouble(0, 1);
    
    return Rayd(ray_orig, ray_dir, ray_time);
}

void RT_Camera::regressionTest() {
    // Test default constructor
    RT_Camera cam1;
    assert(cam1.center == Point3d(0, 0, 0));
    assert(cam1.lookAt == Point3d(0, 0, -1));
    assert(cam1.view_up == Vec3d(0, 1, 0));
    assert(cam1.vertical_fov == 90);
    assert(cam1.defocus_angle == 0);
    assert(cam1.focus_distance == 10);

    // Test parameterized constructor
    RT_Camera cam2(
        Point3d(0, 0, 0),
        Point3d(0, 0, -1),
        Vec3d(0, 1, 0),
        90.0, 0.0, 10.0
    );

    // Test initialize sets derived members correctly
    cam2.initialize(16.0/9.0, 800, 450, 4);
    assert(cam2.sqrt_spp == 2);
    assert(std::abs(cam2.recip_sqrt_spp - 0.5) < 1e-6);

    // Test generateRay returns non-degenerate ray
    Rayd ray = cam2.generateRay(0, 0, 0, 0);
    assert(ray.getDirection().length() > 0);

    // Center pixel should point roughly toward -Z
    Rayd center_ray = cam2.generateRay(400, 225, 0, 0);
    assert(dot(normalize(center_ray.getDirection()), Vec3d(0, 0, -1)) > 0.9);

    std::cout << "All RT_Camera tests passed successfully\n";
}