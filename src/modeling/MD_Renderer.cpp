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

        shader.setVec3 ("uLightPositions[" + idx + "]", Vec3f(viewPos.x, viewPos.y, viewPos.z));
        shader.setVec3 ("uLightColors[" + idx + "]", toVec3<float>(lights[i]->getColor()));
        shader.setFloat("uLightIntensities[" + idx + "]", lights[i]->getIntensity());
    }

    // draw objects
    auto& objects = scene.getObjects();
    for (int i = 0; i < (int)objects.size(); i++) {
        shader.setBool("uSelected", i == scene.selected_obj_index && scene.show_selected);
        drawObject(objects[i], shader, view, proj);
    }

    // ── light gizmos ──────────────────────────────────
    shader.setFloat("uAmbient", 1.0f); // full brightness, unlit
    for (int i = 0; i < (int)lights.size(); i++) {
        drawLightGizmo(lights[i], shader, view);
    }
    shader.setFloat("uAmbient", 0.15f); // restore

    // Axis gizmo
    drawAxisGizmo(shader, view, proj);
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

void MD_Renderer::drawLightGizmo(MD_PointLight* light, MD_Shader& shader, const Mat4f& view) {
    // update gizmo position and scale
    light_gizmo_obj.trs.setTranslation(light->getPosition());
    light_gizmo_obj.trs.setScale(Vec3f(0.08f, 0.08f, 0.08f));

    // set color to match the light
    Vec3f col = toVec3<float>(light->getColor());
    light_gizmo_mat.setColor(col); // you may need to add this setter

    // draw
    Mat4f model = light_gizmo_obj.getTransformMatrix();
    Mat4f localToView = view * model;
    shader.setMat4("uLocalToView", localToView);
    light_gizmo_mat.bind(shader);
    light_gizmo_obj.draw(shader);
}

void MD_Renderer::drawAxisGizmo(MD_Shader& shader, const Mat4f& view, const Mat4f& proj) {
    static float axes[] = {
        // pos         // normal   // uv
        0,0,0,         0,1,0,     0,0,
        1,0,0,         0,1,0,     0,0,
        0,0,0,         0,1,0,     0,0,
        0,1,0,         0,1,0,     0,0,
        0,0,0,         0,1,0,     0,0,
        0,0,1,         0,1,0,     0,0,
    };

    static unsigned int VAO = 0, VBO = 0;
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);
    }


    shader.setMat4("uLocalToView", view * Mat4f::scale(Vec3f(100.f, 100.f, 100.f)));
    shader.setMat4 ("uProjection", proj);  // use standard proj
    shader.setInt  ("u_materialType", 1);
    shader.setFloat("uAmbient", 1.0f);

    glBindVertexArray(VAO);
    glLineWidth(2.0f);

    shader.setVec3("u_color", Vec3f(1.f, 0.2f, 0.2f));
    glDrawArrays(GL_LINES, 0, 2); // X

    shader.setVec3("u_color", Vec3f(0.2f, 1.f, 0.2f));
    glDrawArrays(GL_LINES, 2, 2); // Y

    shader.setVec3("u_color", Vec3f(0.2f, 0.2f, 1.f));
    glDrawArrays(GL_LINES, 4, 2); // Z

    glLineWidth(1.0f);
    glBindVertexArray(0);

    // restore
    shader.setFloat("uAmbient", 0.15f);
    shader.setInt  ("u_materialType", 2);
}

void MD_Renderer::initPickBuffer(int w, int h) {
    if (pick_FBO) {
        glDeleteFramebuffers(1, &pick_FBO);
        glDeleteTextures(1, &pick_tex);
        glDeleteRenderbuffers(1, &pick_depth);
    }
    pick_w = w;
    pick_h = h;

    glGenFramebuffers(1, &pick_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pick_FBO);

    glGenTextures(1, &pick_tex);
    glBindTexture(GL_TEXTURE_2D, pick_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pick_tex, 0);

    glGenRenderbuffers(1, &pick_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, pick_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pick_depth);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int MD_Renderer::pickAt(MD_Scene& scene, int mouse_x, int mouse_y,
                         int vp_x, int vp_y, int vp_w, int vp_h) {
    if (!pick_FBO || pick_w != vp_w || pick_h != vp_h)
        initPickBuffer(vp_w, vp_h);

    Mat4f view = camera_main.getViewMatrix();
    Mat4f proj = camera_main.getProjectionMatrix();

    // render ID pass
    glBindFramebuffer(GL_FRAMEBUFFER, pick_FBO);
    glViewport(0, 0, vp_w, vp_h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    pick_shader.apply();
    pick_shader.setMat4("uProjection", proj);

    // objects — IDs 1..N
    auto& objects = scene.getObjects();
    for (int i = 0; i < (int)objects.size(); i++) {
        if (!objects[i]->selectable) continue;
        Mat4f ltv = view * objects[i]->getTransformMatrix();
        pick_shader.setMat4("uLocalToView", ltv);
        pick_shader.setInt ("uObjectID", i + 1);
        objects[i]->draw(pick_shader);
    }

    // lights — IDs LIGHT_ID_OFFSET+1..LIGHT_ID_OFFSET+M
    auto& lights = scene.getPointLights();
    for (int i = 0; i < (int)lights.size(); i++) {
        Mat4f model = Mat4f::translation(lights[i]->getPosition())
                    * Mat4f::scale(Vec3f(0.8f, 0.8f, 0.8f));
        Mat4f ltv = view * model;
        pick_shader.setMat4("uLocalToView", ltv);
        pick_shader.setInt ("uObjectID",    LIGHT_ID_OFFSET + i + 1);
        MD_Object p_light_gizmo_obj(&pick_light_gizmo);
        p_light_gizmo_obj.draw(pick_shader); // draw raw mesh, no material needed
    }

    // read pixel 
    // flip y: GL origin = bottom-left, SDL origin = top-left
    int gl_x = mouse_x - vp_x;
    int gl_y = vp_h - (mouse_y - vp_y) - 1;

    unsigned char pixel[4] = {0};
    glReadPixels(gl_x, gl_y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int id = pixel[0];
    if (id == 0) return -1;  // background
    if (id <= (int)objects.size()) return id - 1;  // object index
    return LIGHT_ID_OFFSET + (id - LIGHT_ID_OFFSET - 1); // light index
}
