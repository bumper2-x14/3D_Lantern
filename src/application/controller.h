#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "inputs.h"
#include "modeling/MD_Camera.h"
#include "modeling/MD_Object.h"
#include "modeling/MD_Scene.h"

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
                            MD_Object* selected_obj, MD_Scene& scene, int picked, float dt);

    private:
        CtrlMode mode = CtrlMode::CAMERA;
        AxisOfChange axis = AxisOfChange::NONE;

        // Mode handlers
        void updateCamera(const Input& in, MD_Camera& cam, float dt);
        void updateTranslate(const Input& in, MD_Object* selected_obj, float dt);
        void updateRotate(const Input& in, MD_Object* selected_obj, float dt);
        void updateScale(const Input& in, MD_Object* selected_obj, float dt);
        void updateTranslateLight(const Input& in, MD_Scene& scene, float dt);

        void handleSelection(const Input& in, MD_Scene& scene, int picked);

        void modeSwitcher(const Input& in);
        void axisSwitcher(const Input& in);

};


#endif