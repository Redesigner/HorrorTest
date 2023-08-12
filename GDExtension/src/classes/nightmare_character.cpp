#include "nightmare_character.h"

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/core/math.hpp>

#include "../core/custom_math.h"

using namespace godot;

void NightmareCharacter::_bind_methods()
{
    // BIND_PROPERTY(lookSpeedVertical, NightmareCharacter, Variant::FLOAT);
}

void NightmareCharacter::_update_process_callback()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
		set_process_internal(false);
		set_physics_process_internal(false);
        set_process_input(false);
	}
    else
    {
        set_process_mode(PROCESS_MODE_INHERIT);
		set_process_internal(true);
		set_physics_process_internal(true);
        set_process_input(true);
	}
}

void NightmareCharacter::_notification(int p_what)
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

NightmareCharacter::NightmareCharacter()
{
}

NightmareCharacter::~NightmareCharacter()
{
}

void NightmareCharacter::_ready()
{
    _cameraArm = dynamic_cast<Node3D*>(get_node_internal("CameraArm"));
    _inputVectorDisplay = dynamic_cast<Node3D*>(get_node_or_null("InputVectorDisplay"));
    _playerMesh = dynamic_cast<MeshInstance3D*>(get_node_or_null("PlayerMesh"));
    _interactVolume = dynamic_cast<Area3D*>(get_node_or_null("PlayerMesh/InteractVolume"));

    _debugText = dynamic_cast<RichTextLabel*>(get_node_or_null("DebugText"));

    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
}

void NightmareCharacter::_input(const Ref<InputEvent> &event)
{
    if (event->is_class("InputEventMouseMotion"))
    {
        InputEventMouseMotion *mouseMotionEvent = static_cast<InputEventMouseMotion *>(*event);
        rotate_camera(mouseMotionEvent->get_relative());
        return;
    }

    if (event->is_action_pressed("ui_cancel"))
    {
        get_tree()->quit();
        return;
    }

    if (event->is_action_pressed("interact"))
    {
        interact();
        return;
    }
}

void NightmareCharacter::_process(double delta)
{
}

void NightmareCharacter::_physics_process(double delta)
{
    update_input();

    Vector3 velocity = get_velocity();
    if (input_pressed())
    {
        Vector2 desiredVelocity = _maxWalkSpeed * _inputVector;
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
    velocity += Vector3(0.0f, -9.8f * delta, 0.0f);
    set_velocity(velocity);
    move_and_slide();
    String text = String("Velocity: '{0}' m/s").format(Array::make(velocity.length()));
    if (input_pressed())
    {
        text += "\nInput currently pressed.";
    }
    if (is_on_floor())
    {
        text += "\nCurrently on the floor.";
    }

    look_at_walk_direction(delta);
    if (get_position().y < -100.0f)
    {
        set_position(Vector3(0.0f, 5.0f, 0.0f));
        set_velocity(Vector3(0.0f, 0.0f, 0.0f));
    }
    _debugText->set_text(text);
}

void NightmareCharacter::rotate_camera(Vector2 input)
{
    if (!_cameraArm)
    {
        return;
    }
    Vector3 currentRotation = _cameraArm->get_rotation_degrees();
    currentRotation.y += input.x * -_lookSpeedHorizontal;
    currentRotation.x += input.y * -_lookSpeedVertical;
    currentRotation.x = Math::clamp(currentRotation.x, -89.9f, 89.9f);
    _cameraArm->set_rotation_degrees(currentRotation);
}

Vector2 NightmareCharacter::get_input_vector() const
{
    return Input::get_singleton()->get_vector("walk_left", "walk_right", "walk_forward", "walk_back");
}

void NightmareCharacter::update_input()
{
    Vector2 inputVector = get_input_vector();
    Vector3 cameraRotation = _cameraArm->get_rotation();
    float cameraYaw = -cameraRotation.y;
    float cos = Math::cos(cameraYaw);
    float sin = Math::sin(cameraYaw);
    _inputVector = Vector2(cos * inputVector.x - sin * inputVector.y, sin * inputVector.x + cos * inputVector.y);

    _inputVectorDisplay->set_position(Vector3(_inputVector.x, _inputVectorDisplay->get_position().y, _inputVector.y));

    Vector2 cameraInputVector = Input::get_singleton()->get_vector("look_left", "look_right", "look_up", "look_down");
    rotate_camera(cameraInputVector);
}

bool NightmareCharacter::input_pressed() const
{
	return !(_inputVector.x == 0.0f && _inputVector.y == 0.0f);
}

void NightmareCharacter::look_at_walk_direction(double delta)
{
    if (!_playerMesh)
    {
        return;
    }
    if (!input_pressed())
    {
        return;
    }
    Vector2 normalizedDirection = _inputVector.normalized();
    float desiredYaw = Math::rad_to_deg(Math::atan2(normalizedDirection.y, normalizedDirection.x));
    float deltaYaw = CustomMath::angle_between(-_playerMesh->get_rotation_degrees().y - 90.0f, desiredYaw);
    float yawOffset = Math::sign(deltaYaw) * _turnSpeed * (float)delta;

    // Clamp our rotation this frame so that its magnitude is never larger than the difference between our desired rotation and current rotation
    if (Math::abs(yawOffset) > Math::abs(deltaYaw))
    {
        yawOffset = deltaYaw;
    }


    Vector3 postPhysicsRotationDegrees = _playerMesh->get_rotation_degrees();
    postPhysicsRotationDegrees.y -= yawOffset;
    _playerMesh->set_rotation_degrees(postPhysicsRotationDegrees);
}

void NightmareCharacter::interact()
{
    /* TypedArray<Area3D> hitVolumes = _interactVolume->get_overlapping_areas();
    for(int i = 0; i < hitVolumes.size() - 1; i++)
    {
        Area3D volume = hitVolumes.;
        if (volume.Owner is not NPC)
        {
            continue;
        }

        NPC hitNpc = (NPC)volume.Owner;
        hitNpc.TriggerInteraction(this);
    } */
}

void SetDialog(String dialog)
{
    // EmitSignal(SignalName.DialogChanged, dialog);
}