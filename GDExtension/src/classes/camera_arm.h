#ifndef NIGHTMARE_CAMERA_ARM_H
#define NIGHTMARE_CAMERA_ARM_H

#include <godot_cpp/classes/spring_arm3d.hpp>

#include "../class_macros.h"

namespace godot
{
// A custom version of the springarm3d that lets us toggle between two states,
// so the properties are grouped together properly for use in the editor
class CameraArm : public SpringArm3D
{
    GDCLASS(CameraArm, SpringArm3D);

public:
    CameraArm();
    ~CameraArm();

    void set_focus(bool focus);
    bool get_focus() const;
    

protected:
    static void _bind_methods();

private:
    // are we in 'focused' mode, which uses alternative properties
    bool _focus;

    DECLARE_PROPERTY(float, focusedArmLength, 1.0f);
    DECLARE_PROPERTY(float, unfocusedArmLength, 2.0f);
};
}

#endif