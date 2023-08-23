#include "camera_arm.h"

using namespace godot;

CameraArm::CameraArm()
{
}

CameraArm::~CameraArm()
{
}

void CameraArm::_bind_methods()
{
    BIND_PROPERTY(Variant::BOOL, focus, CameraArm);
    BIND_PROPERTY_HINT(Variant::FLOAT, focusedArmLength, CameraArm, PROPERTY_HINT_RANGE, "0,100,0.1,suffix:m");
    BIND_PROPERTY_HINT(Variant::FLOAT, unfocusedArmLength, CameraArm, PROPERTY_HINT_RANGE, "0,100,0.1,suffix:m");
}

void CameraArm::set_focus(bool focus)
{
    _focus = focus;
    set_length(focus ? _focusedArmLength : _unfocusedArmLength);
}

bool CameraArm::get_focus() const
{
    return _focus;
}