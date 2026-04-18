#include "MD_Renderer.h"

MD_Renderer::MD_Renderer() {}

void MD_Renderer::render(MD_Scene& scene, MD_Shader& shader) {
    Mat4f view = camera_main.getViewMatrix();
    Mat4f proj = camera_main.getProjectionMatrix();

    shader.apply();

    // set once per frame
    shader.setMat4("uProjection", proj);
    shader.setVec3("uSelectionTint", Vec3f(1.3f, 1.3f, 0.6f));

    auto& lights = scene.getPointLights();
    shader.setInt("uNbLights", (int)lights.size());
    for (int i = 0; i < (int)lights.size(); i++) {
        std::string idx = std::to_string(i);

        // transform position to view space to match v_fragPos space
        Vec3f worldPos = lights[i]->getPosition();
        Vec4f viewPos  = view * Vec4f(worldPos.x, worldPos.y, worldPos.z, 1.f);

        shader.setVec3 ("uLightPositions["   + idx + "]", Vec3f(viewPos.x, viewPos.y, viewPos.z));
        shader.setVec3 ("uLightColors["      + idx + "]", lights[i]->getColor());
        shader.setFloat("uLightIntensities[" + idx + "]", lights[i]->getIntensity());
    }

    // draw objects
    auto& objects = scene.getObjects();
    for (int i = 0; i < (int)objects.size(); i++) {
        shader.setBool("uSelected", i == scene.selected_index && scene.show_selected);
        drawObject(objects[i], shader, view, proj);
    }
}

void MD_Renderer::drawObject(MD_Object* obj, MD_Shader& shader,
                              const Mat4f& view, const Mat4f& proj) {
    if (!obj) return;

    Mat4f model = obj->getTransformMatrix();
    Mat4f localToView = view * model;
    // pass the full mat4 — the shader extracts mat3 and computes the normal matrix
    shader.setMat4("uLocalToView", localToView);
    
    if (obj->getMaterial())
        obj->getMaterial()->bind(shader);

    obj->draw(shader);
}
