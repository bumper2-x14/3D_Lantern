#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "inputs.h"
#include "modeling/MD_Camera.h"
#include "modeling/MD_Object.h"

enum class CtrlMode {
    CAMERA,
    TRANSLATE,
    SCALE,
    ROTATE
};

enum class AxisOfChange { NONE, X, Y, Z };

class Controller {
    public:
        void ctrlUpdate(const Input& in, MD_Camera& cam, 
                            MD_Object* selected_obj, float dt);

    private:
        CtrlMode mode = CtrlMode::CAMERA;
        AxisOfChange axis = AxisOfChange::NONE;

        // Mode handlers
        void updateCamera(const Input&, MD_Camera&, float dt);
        void updateTranslate(const Input&, MD_Object*, float dt);
        void updateRotate(const Input&, MD_Object*, float dt);
        void updateScale(const Input&, MD_Object*, float dt);

        void modeSwitcher(const Input& in);
        void axisSwitcher(const Input& in);
};


#endif