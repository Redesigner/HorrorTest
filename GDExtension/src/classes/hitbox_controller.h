#ifndef NIGHTMARE_HITBOX_CONTROLLER_H
#define NIGHTMARE_HITBOX_CONTROLLER_H

#include <godot_cpp/classes/area3d.hpp>

#include <godot_cpp/classes/collision_shape3d.hpp>

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

    void spawn_hitbox(Vector3 offset, Ref<Shape3D> hitboxShape, String hitboxName);
    
    void remove_hitbox(String hitboxName);
    
private:
    int get_hitbox(String hitboxName) const;

private:
    TypedArray<CollisionShape3D> _hitboxes;
};
}

#endif