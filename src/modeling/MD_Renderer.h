#ifndef MD_RENDERER_H
#define MD_RENDERER_H

#include "MD_Scene.h"
#include "MD_Camera.h"
#include "MD_Shader.h"
#include "MD_Sphere.h"
#include "MD_Material.h"

class MD_Renderer {
public:
    static constexpr int LIGHT_ID_OFFSET = 10000;

    MD_Renderer();

    // cameras
    MD_Camera& getCameraMain() { return camera_main; }
    // draw 
    void drawLightGizmo(MD_PointLight* light, MD_Shader& shader, const Mat4f& view);
    void drawAxisGizmo(MD_Shader& shader, const Mat4f& view, const Mat4f& proj);
    void render(MD_Scene& scene, MD_Shader& shader);

    // returns: >= 0 object index, >= LIGHT_ID_OFFSET light, -1 background
    int pickAt(MD_Scene& scene, int mouse_x, int mouse_y,
           int vp_x, int vp_y, int vp_w, int vp_h);


private:
    MD_Camera camera_main;
    MD_Sphere light_gizmo_shape{4, 4};
    MD_Material light_gizmo_mat{Vec3f(1.f, 1.f, 1.f), MD_Material::MatType::AMBIENT};
    MD_Object light_gizmo_obj{"Light_gizmo", &light_gizmo_shape, &light_gizmo_mat};

    unsigned int pick_FBO = 0;
    unsigned int pick_tex = 0;
    unsigned int pick_depth = 0;
    int pick_w = 0;
    int pick_h = 0;
    MD_Shader pick_shader{SHADER_DIR "pick_shader.vs", SHADER_DIR "pick_shader.fs"};
    MD_Sphere pick_light_gizmo{4, 4};

    void drawObject(MD_Object* obj, MD_Shader& shader, const Mat4f& view, const Mat4f& proj);
    void initPickBuffer(int w, int h);
};

#endif
