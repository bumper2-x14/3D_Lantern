#ifndef RT_RENDERER_H
#define RT_RENDERER_H

#include <vector>
#include <fstream>
#include <thread>

#include "RT_Scene.h"
#include "RT_Camera.h"
#include "RT_Object.h"
#include "RT_ObjectList.h"
#include "RT_Light.h"
#include "math/color.h"
#include "RT_BVH.h"

/// @brief Main ray tracing renderer.
///        Handles rendering loop, sampling and output.
class RT_Renderer {
    public:        
        /// @brief Creates a renderer with given settings.
        RT_Renderer(int _img_width, double _aspect_ratio, int _samples_per_pixel, int _depth);

        /// @brief Destructor (frees image buffer).
        ~RT_Renderer();

        // setup

        /// @brief Sets the camera.
        void setCamera(RT_Camera* cam);

        /// @brief Sets the scene.
        void setScene(RT_Scene* scene);

        /// @brief Sets background color.
        void setBackground(const Color& bg);

        // rendering

        /// @brief Starts rendering (single or multi-threaded).
        void render(bool threaded = false);

        /// @brief Writes the rendered image to a PPM file.
        void writePPM(const std::string& path) const;

        /// @brief Runs basic tests.
        static void regressionTest();
        
    private:
        Color* img_buffer; ///< Image buffer (width * height).
        RT_Camera* camera; ///< Active camera.
        int sample_per_pixel; ///< Number of samples per pixel.
        int depth; ///< Max recursion depth.
        Color background; ///< Background color.
        double aspect_ratio; ///< Aspect ratio.
        int img_width; ///< Image width.
        int img_height; ///< Image height.
        int sqrt_spp; ///< sqrt(samples per pixel) (for stratified sampling).
        double sample_scale; ///< Scale factor for averaging samples.
        RT_Scene* scene = nullptr; ///< Scene to render.

        /// @brief Traces a ray recursively.
        Color traceRay(const Rayd& ray, int recursive_depth, RT_Object* accel) const; 

        /// @brief Single-threaded rendering.
        void singleThreadRender();

        /// @brief Multi-threaded rendering.
        void multiThreadRender();

        /// @brief Worker function for threads.
        void renderWorker(int start_y, int end_y, RT_Object* accel, std::atomic<int>& done);

};

#endif