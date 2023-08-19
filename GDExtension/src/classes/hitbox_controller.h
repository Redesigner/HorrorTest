#ifndef NIGHTMARE_HITBOX_CONTROLLER_H
#define NIGHTMARE_HITBOX_CONTROLLER_H

#include <godot_cpp/classes/area3d.hpp>

namespace godot
{
class HitboxController : public Area3D
{
    GDCLASS(HitboxController, Area3D);

protected:
    static void _bind_methods();

public:
    HitboxController();
    ~HitboxController();

    virtual void _ready() override;

    void spawn_hitbox(Vector3 offset, class Shape3D *hitboxShape, String hitboxName);
    
};
}

#endif