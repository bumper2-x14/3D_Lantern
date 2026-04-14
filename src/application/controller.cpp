#include "controller.h"

void Controller::ctrlUpdate(const Input& in, MD_Camera& cam, 
                            MD_Object* selected_obj, MD_Scene& scene, float dt) {

    handleSelection(in, scene);

    modeSwitcher(in);
    axisSwitcher(in);

    switch (mode) {
        case CtrlMode::CAMERA:
            updateCamera(in, cam, dt);
            break;

        case CtrlMode::TRANSLATE:
            updateTranslate(in, selected_obj, dt);
            break;

        case CtrlMode::ROTATE:
            updateRotate(in, selected_obj, dt);
            break;

        case CtrlMode::SCALE:
            updateScale(in, selected_obj, dt);
            break;
    }
}

void Controller::updateCamera(const Input& in, MD_Camera& cam, float dt) {
    MD_Camera::CameraInput ci{};

    ci.w = in.isKeyDown(SDL_SCANCODE_UP);
    ci.s = in.isKeyDown(SDL_SCANCODE_DOWN);
    ci.a = in.isKeyDown(SDL_SCANCODE_LEFT);
    ci.d = in.isKeyDown(SDL_SCANCODE_RIGHT);
    ci.up = in.isKeyDown(SDL_SCANCODE_SPACE);
    ci.down = in.isKeyDown(SDL_SCANCODE_LALT);

    ci.speed = 4.0f * dt;

    // sprint combo
    if (in.isKeyDown(SDL_SCANCODE_LSHIFT))
        ci.speed *= 3.0f;

    cam.update(ci);

    if (in.isMouseDown(SDL_BUTTON_LEFT)) {
        MD_Camera::CameraInput mi{};
        mi.xoffset = -in.getDeltaX();
        mi.yoffset = -in.getDeltaY();
        mi.speed = 5.f;
        cam.update(mi);
    }
}

void Controller::updateTranslate(const Input& in, MD_Object* selected_obj, float dt) {
    if (!selected_obj) return;

    float mv = 5.0f * dt;

    if (in.isKeyDown(SDL_SCANCODE_LCTRL))
        mv *= 0.2f; // precision

    float dir = 0.f;

    if (in.isKeyDown(SDL_SCANCODE_UP))
        dir += 1.f;

    if (in.isKeyDown(SDL_SCANCODE_DOWN))
        dir -= 1.f;

    Vec3f delta{0.f, 0.f, 0.f};

    switch (axis) {
        case AxisOfChange::X: delta.x = dir * mv; break;
        case AxisOfChange::Y: delta.y = dir * mv; break;
        case AxisOfChange::Z: delta.z = dir * mv; break;

        case AxisOfChange::NONE:
            // free movement
            if (in.isKeyDown(SDL_SCANCODE_LEFT))  delta.x -= mv;
            if (in.isKeyDown(SDL_SCANCODE_RIGHT)) delta.x += mv;
            if (in.isKeyDown(SDL_SCANCODE_UP))    delta.y += mv;
            if (in.isKeyDown(SDL_SCANCODE_DOWN))  delta.y -= mv;
            break;
    }
    selected_obj->trs.translateBy(delta);
}

void Controller::updateRotate(const Input& in, MD_Object* selected_obj, float dt) {
    if (!selected_obj) return;

    float rv = 5.0f * dt;

    if (in.isKeyDown(SDL_SCANCODE_LCTRL))
        rv *= 0.2f;

    float dir = 0.f;

    if (in.isKeyDown(SDL_SCANCODE_UP)) dir += 1.f;
    if (in.isKeyDown(SDL_SCANCODE_DOWN)) dir -= 1.f;

    Vec3f delta{0.f, 0.f, 0.f};

    switch (axis) {
        case AxisOfChange::X: delta.x = dir * rv; break;
        case AxisOfChange::Y: delta.y = dir * rv; break;
        case AxisOfChange::Z: delta.z = dir * rv; break;

        case AxisOfChange::NONE:
            delta.y = dir * rv; // default axis
            break;
    }
    selected_obj->trs.rotateBy(delta);
}

void Controller::updateScale(const Input& in, MD_Object* selected_obj, float dt) {
    if (!selected_obj) return;

    float sv = 1.0f * dt;
    if (in.isKeyDown(SDL_SCANCODE_LCTRL))
        sv *= 0.2f;

    float dir = 0.f;

    if (in.isKeyDown(SDL_SCANCODE_UP)) dir += 1.f; // grow
    if (in.isKeyDown(SDL_SCANCODE_DOWN)) dir -= 1.f; // shrink

    Vec3f factor{1.f, 1.f, 1.f};

    switch (axis) {
        case AxisOfChange::X: factor.x += dir * sv; break;
        case AxisOfChange::Y: factor.y += dir * sv; break;
        case AxisOfChange::Z: factor.z += dir * sv; break;

        case AxisOfChange::NONE:
            factor = Vec3f(1.f + dir * sv,
                           1.f + dir * sv,
                           1.f + dir * sv);
            break;
    }
    selected_obj->trs.scaleBy(factor);
}

void Controller::handleSelection(const Input& in, MD_Scene& scene) {
    if (in.isKeyPressed(SDL_SCANCODE_TAB)) {
        scene.nextObject();
        std::cout << "Selected object: " << scene.selected_index << '\n';
    }
}

void Controller::modeSwitcher(const Input& in) {
    if(in.isKeyPressed(SDL_SCANCODE_T)){
        mode = CtrlMode::TRANSLATE;
        axis = AxisOfChange::NONE;
        std::cout << "[MODE] TRANSLATE\n";
    }
    else if(in.isKeyPressed(SDL_SCANCODE_S)) {
        mode = CtrlMode::SCALE;
        axis = AxisOfChange::NONE;
        std::cout << "[MODE] SCALE\n";
    }
    else if(in.isKeyPressed(SDL_SCANCODE_R)) {
        mode = CtrlMode::ROTATE;
        axis = AxisOfChange::NONE;
        std::cout << "[MODE] ROTATE\n";
    }
    else if (in.isKeyPressed(SDL_SCANCODE_P)) {
        mode = CtrlMode::CAMERA;
        axis = AxisOfChange::NONE;
        std::cout << "[MODE] CAMERA\n";
    }
}

void Controller::axisSwitcher(const Input& in) {
    if(in.isKeyPressed(SDL_SCANCODE_X)) {
        axis = AxisOfChange::X;
        std::cout << "[AXIS] X\n";
    }
    else if(in.isKeyPressed(SDL_SCANCODE_Y)) {
        axis = AxisOfChange::Y;
        std::cout << "[AXIS] Y\n";
    }
    else if(in.isKeyPressed(SDL_SCANCODE_Z)) {
        axis = AxisOfChange::Z;
        std::cout << "[AXIS] Z\n";
    }
    else if(in.isKeyPressed(SDL_SCANCODE_N)) {
        axis = AxisOfChange::NONE;
        std::cout << "[AXIS] NONE\n";
    }
}

