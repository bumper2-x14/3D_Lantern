#ifndef RT_RENDERER_H
#define RT_RENDERER_H

#include <vector>

#include "RT_Camera.h"
#include "RT_ObjectList.h"
#include "math/color.h"

class RT_Renderer {
    public:
        
    private:
        Color* img_buffer;
        RT_Camera* camera;
        RT_ObjectList* scene;
        int sample_per_pixel;
        Color background;
        double aspect_ratio;
        int img_width;
        int img_height;
};

#endif