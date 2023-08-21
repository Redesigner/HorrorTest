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
#include "npc.h"
#include "ui/nightmare_ui.h"

using namespace godot;

void NightmareCharacter::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::FLOAT, lookSpeedVertical, NightmareCharacter, PROPERTY_HINT_RANGE, "0,30,0.1");
    BIND_PROPERTY_HINT(Variant::FLOAT, lookSpeedHorizontal, NightmareCharacter, PROPERTY_HINT_RANGE, "0,30,0.01");

    ADD_SIGNAL(MethodInfo("dialog_changed", PropertyInfo(Variant::STRING, "dialog")));
}

NightmareCharacter::NightmareCharacter()
{
    _cameraArm = nullptr;
    _inputVectorDisplay = nullptr;
    _interactVolume = nullptr;
    _debugText = nullptr;
}

NightmareCharacter::~NightmareCharacter()
{
}

void NightmareCharacter::_ready()
{
    Pawn::_ready();
    _cameraArm = dynamic_cast<Node3D*>(get_node_internal("CameraArm"));
    _inputVectorDisplay = dynamic_cast<Node3D*>(get_node_or_null("InputVectorDisplay"));
    _interactVolume = dynamic_cast<Area3D*>(get_node_or_null("PlayerMesh/InteractVolume"));

    _debugText = dynamic_cast<RichTextLabel*>(get_node_or_null("DebugText"));

    NightmareUi* ui = dynamic_cast<NightmareUi *>(get_node_internal("ActiveUI"));
    connect("dialog_changed",  Callable(ui, "set_dialog"));

    if (!_inEditor)
    {
        Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
    }
}

void NightmareCharacter::_input(const Ref<InputEvent> &event)
{
    if (_inEditor)
    {
        return;
    }

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
    if (_inEditor)
    {
        return;
    }

    update_input();
    Pawn::_physics_process(delta);
    
    Vector3 velocity = get_velocity();
    String text = String("Velocity: '{0}' m/s").format(Array::make(velocity.length()));
    if (input_pressed())
    {
        text += "\nInput currently pressed.";
    }
    if (is_on_floor())
    {
        text += "\nCurrently on the floor.";
    }

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

void NightmareCharacter::interact()
{
    TypedArray<Area3D> hitVolumes = _interactVolume->get_overlapping_areas();
    for(int i = 0; i < hitVolumes.size(); i++)
    {
        Object *volumeObject = hitVolumes[i];
        Area3D *volume = Object::cast_to<Area3D>(volumeObject);
        if (!volume->get_owner()->is_class("NPC"))
        {
            continue;
        }

        NPC* hitNpc = dynamic_cast<NPC *>(volume->get_owner());
        hitNpc->trigger_interaction(this);
    }
}

void NightmareCharacter::set_dialog(String dialog)
{
    emit_signal("dialog_changed", dialog);
}