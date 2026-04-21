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


class RT_Renderer {
    public:        
        RT_Renderer(int _img_width, double _aspect_ratio, int _samples_per_pixel, int _depth);
        ~RT_Renderer();

        // setup
        void setCamera(RT_Camera* cam);
        void setScene(RT_Scene* scene);
        void setBackground(const Color& bg);

        // rendering
        void render(bool threaded = false);
        void writePPM(const std::string& path) const;

        static void regressionTest();
        
    private:
        Color* img_buffer;
        RT_Camera* camera;
        int sample_per_pixel;
        int depth;
        Color background;
        double aspect_ratio;
        int img_width;
        int img_height;
        int sqrt_spp;
        double sample_scale;
        RT_Scene* scene = nullptr;

        Color traceRay(const Rayd& ray, int recursive_depth, RT_Object* accel) const; 
        void singleThreadRender();
        void multiThreadRender();
        void renderWorker(int start_y, int end_y, RT_Object* accel, std::atomic<int>& done);

};

#endif