#include "hitbox_controller.h"

#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/label3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void HitboxController::_bind_methods()
{
}

HitboxController::HitboxController()
{
}

HitboxController::~HitboxController()
{
}

void HitboxController::_ready()
{
    spawn_hitbox(Vector3(0.0f, 0.0f, 0.0f), nullptr, "Test");
}

void HitboxController::spawn_hitbox(Vector3 offset, Shape3D *hitboxShape, String hitboxName)
{
    UtilityFunctions::print("Spawning hitbox");
    CollisionShape3D *hitbox  = memnew(CollisionShape3D);
    add_child(hitbox);
}