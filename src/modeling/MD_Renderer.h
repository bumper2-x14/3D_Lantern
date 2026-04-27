#ifndef MD_RENDERER_H
#define MD_RENDERER_H

#include "MD_Scene.h"
#include "MD_Camera.h"
#include "MD_Shader.h"
#include "MD_Sphere.h"
#include "MD_Material.h"

/// @brief Handles rendering of the scene (objects, lights, gizmos, picking).
class MD_Renderer {
public:
    /// @brief Default constructor.
    MD_Renderer();

    // cameras

    /// @brief Returns the main camera used for rendering.
    MD_Camera& getCameraMain() { return camera_main; }

    // draw 

    /// @brief Draws a small sphere to represent a light in the scene.
    /// @param light Pointer to the light.
    /// @param shader Shader used for drawing.
    /// @param view Current view matrix.
    void drawLightGizmo(MD_PointLight* light, MD_Shader& shader, const Mat4f& view);

    /// @brief Draws axis gizmo (usually XYZ axes for orientation).
    /// @param shader Shader used for drawing.
    /// @param view View matrix.
    /// @param proj Projection matrix.
    void drawAxisGizmo(MD_Shader& shader, const Mat4f& view, const Mat4f& proj);

    /// @brief Renders the full scene (objects + lights).
    /// @param scene Scene to render.
    /// @param shader Shader used for rendering.
    void render(MD_Scene& scene, MD_Shader& shader);

    /// @brief Picks an object/light based on mouse position.
    /// @return >= 0 object index, >= LIGHT_ID_OFFSET light, -1 background
    int pickAt(MD_Scene& scene, int mouse_x, int mouse_y,
           int vp_x, int vp_y, int vp_w, int vp_h);

private:
    MD_Camera camera_main; ///< Main camera used for rendering.

    // light gizmo (small sphere used to visualize light position)
    MD_Sphere light_gizmo_shape{4, 4};
    MD_Material light_gizmo_mat{"light_gizmo" , Vec3f(1.f, 1.f, 1.f), MD_Material::MatType::AMBIENT};
    MD_Object light_gizmo_obj{"Light_gizmo", &light_gizmo_shape, &light_gizmo_mat};

    // picking buffers (used to detect clicked object)
    unsigned int pick_FBO = 0; ///< Framebuffer for picking.
    unsigned int pick_tex = 0; ///< Texture storing object IDs.
    unsigned int pick_depth = 0; ///< Depth buffer for picking.
    int pick_w = 0; ///< Width of picking buffer.
    int pick_h = 0; ///< Height of picking buffer.

    MD_Shader pick_shader{SHADER_DIR "pick_shader.vs", SHADER_DIR "pick_shader.fs"}; ///< Shader used for picking pass.
    MD_Sphere pick_light_gizmo{4, 4}; ///< Sphere used during picking for lights.

    /// @brief Draws a single object with given matrices.
    void drawObject(MD_Object* obj, MD_Shader& shader, const Mat4f& view, const Mat4f& proj);

    /// @brief Initializes or resizes the picking framebuffer.
    void initPickBuffer(int w, int h);
};

#endif