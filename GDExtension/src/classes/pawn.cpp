#include "pawn.h"

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/core/math.hpp>

#include "../core/custom_math.h"

using namespace godot;

void Pawn::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::FLOAT, maxWalkSpeed, Pawn, PROPERTY_HINT_RANGE, "0,100,0.1,suffix:m/s");
    BIND_PROPERTY_HINT(Variant::FLOAT, groundAcceleration, Pawn, PROPERTY_HINT_RANGE, "0,100,0.1,suffix:m/s2");
    BIND_PROPERTY_HINT(Variant::FLOAT, groundFriction, Pawn, PROPERTY_HINT_RANGE, "0,100,0.1,suffix:m/s2");
    BIND_PROPERTY_HINT(Variant::FLOAT, turnSpeed, Pawn, PROPERTY_HINT_RANGE, "0,720,1,suffix:deg/s");
}

void Pawn::_update_process_callback()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        _inEditor = true;
		set_process(false);
		set_physics_process_internal(false);
        set_process_input(false);
	}
    else
    {
        _inEditor = false;
		set_process_internal(true);
		set_physics_process_internal(true);
        set_process_input(true);
	}
}

void Pawn::_notification(int p_what)
{
    switch(p_what)
    {
        case NOTIFICATION_ENTER_TREE:
        {
            ERR_FAIL_COND(!is_inside_tree());
            _update_process_callback();
        }break;
    }
}

Pawn::Pawn()
{
    _pawnMesh = nullptr;
    _inEditor = false;
}

Pawn::~Pawn()
{
}

void Pawn::_ready()
{
    _pawnMesh = dynamic_cast<MeshInstance3D*>(get_node_or_null("Mesh"));

    _update_process_callback();
}

void Pawn::_process(double delta)
{
}

void Pawn::_physics_process(double delta)
{
    // Don't run the physics process while we're in the editor
    if (_inEditor)
    {
        return;
    }

    
    Vector3 velocity = get_velocity();
    if (input_pressed())
    {
        Vector2 desiredVelocity = get_max_speed() * _inputVector;
        Vector2 currentGroundVelocity = Vector2(velocity.x, velocity.z);
        Vector2 groundVelocity = CustomMath::constant_interp_to_vector(currentGroundVelocity, desiredVelocity, _groundAcceleration, (float)delta);
        velocity = Vector3(groundVelocity.x, velocity.y, groundVelocity.y);
    }
    else if (is_on_floor())
    {
        Vector2 desiredVelocity = Vector2(0.0f, 0.0f);
        Vector2 currentGroundVelocity = Vector2(velocity.x, velocity.z);
        Vector2 groundVelocity = CustomMath::constant_interp_to_vector(currentGroundVelocity, desiredVelocity, _groundAcceleration, (float)delta);
        velocity = Vector3(groundVelocity.x, velocity.y, groundVelocity.y);
        if (CustomMath::is_nearly_zero(velocity))
        {
            velocity = Vector3(0.0f, 0.0f, 0.0f);
        }
    }
    if (!is_on_floor())
    {
        velocity += Vector3(0.0f, -9.8f * delta, 0.0f);
    }
    look_at_walk_direction(delta);
    set_velocity(velocity);
    move_and_slide();
}

bool Pawn::input_pressed() const
{
	return !(_inputVector.x == 0.0f && _inputVector.y == 0.0f);
}

void Pawn::look_at_walk_direction(double delta)
{
    if (!_pawnMesh)
    {
        return;
    }
    if (!input_pressed())
    {
        return;
    }
    Vector2 normalizedDirection = _inputVector.normalized();
    float desiredYaw = Math::rad_to_deg(Math::atan2(normalizedDirection.y, normalizedDirection.x));
    float deltaYaw = CustomMath::angle_between(-_pawnMesh->get_rotation_degrees().y - 90.0f, desiredYaw);
    float yawOffset = Math::sign(deltaYaw) * _turnSpeed * (float)delta;

    // Clamp our rotation this frame so that its magnitude is never larger than the difference between our desired rotation and current rotation
    if (Math::abs(yawOffset) > Math::abs(deltaYaw))
    {
        yawOffset = deltaYaw;
    }

    Vector3 postPhysicsRotationDegrees = _pawnMesh->get_rotation_degrees();
    postPhysicsRotationDegrees.y -= yawOffset;
    _pawnMesh->set_rotation_degrees(postPhysicsRotationDegrees);
}

float Pawn::get_max_speed() const
{
    return _maxWalkSpeed;
}