#include "hitbox_controller.h"

#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/label3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void HitboxController::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("spawn_hitbox", "offset", "hitbox_shape", "hitbox_name"), &HitboxController::spawn_hitbox);
    ClassDB::bind_method(D_METHOD("remove_hitbox", "hitbox_name"), &HitboxController::remove_hitbox);
}

HitboxController::HitboxController()
{
    _hitboxes = TypedArray<CollisionShape3D>();
}

HitboxController::~HitboxController()
{
    for (int i = _hitboxes.size() - 1; i >= 0; i--)
    {
        Object *hitboxObject = _hitboxes[i];
        CollisionShape3D *hitbox = Object::cast_to<CollisionShape3D>(hitboxObject);
        hitbox->queue_free();
    }
}

void HitboxController::_ready()
{
}

void HitboxController::spawn_hitbox(Vector3 offset, Ref<Shape3D>hitboxShape, String hitboxName)
{
    if (!hitboxShape.is_valid())
    {
        UtilityFunctions::print(String("HitboxController '{0}' failed to spawn, collision shape invalid.").format(Array::make(get_name())));
        return;
    }
    CollisionShape3D *hitbox  = memnew(CollisionShape3D);
    add_child(hitbox);
    hitbox->set_name(hitboxName);
    hitbox->set_shape(hitboxShape);
    _hitboxes.insert(_hitboxes.size(), hitbox);
}

void HitboxController::remove_hitbox(String hitboxName)
{
    int hitboxIndex = get_hitbox(hitboxName);
    if (hitboxIndex == -1)
    {
        return;
    }
    Object *hitboxObject = _hitboxes[hitboxIndex];
    CollisionShape3D *hitbox = Object::cast_to<CollisionShape3D>(hitboxObject);
    hitbox->queue_free();
    _hitboxes.remove_at(hitboxIndex);
}

int HitboxController::get_hitbox(String hitboxName) const
{
    for (int i = 0; i < _hitboxes.size(); i++)
    {
        Object *hitboxObject = _hitboxes[i];
        Shape3D *hitbox = Object::cast_to<Shape3D>(hitboxObject);
        if (hitboxName == hitbox->get_name())
        {
            return i;
        }
    }
    return -1;
}