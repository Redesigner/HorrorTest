#include "nightmare_character.h"

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/core/math.hpp>

#include "../core/custom_math.h"

#include "npc.h"
#include "ui/nightmare_ui.h"
#include "camera_arm.h"
#include "enemy.h"

using namespace godot;

void NightmareCharacter::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::FLOAT, lookSpeedVertical, NightmareCharacter, PROPERTY_HINT_RANGE, "0,30,0.1");
    BIND_PROPERTY_HINT(Variant::FLOAT, lookSpeedHorizontal, NightmareCharacter, PROPERTY_HINT_RANGE, "0,30,0.01");
    BIND_PROPERTY_HINT(Variant::FLOAT, readyWalkSpeed, NightmareCharacter, PROPERTY_HINT_RANGE, "0,10,0.1,suffix:m/s");
    BIND_PROPERTY(Variant::STRING, bulletScenePath, NightmareCharacter);

    ADD_SIGNAL(MethodInfo("dialog_changed", PropertyInfo(Variant::STRING, "dialog")));

    // expose this method to the api for the timer to work
    ClassDB::bind_method(D_METHOD("end_interact_debounce"), &NightmareCharacter::end_interact_debounce);
    ClassDB::bind_method(D_METHOD("end_weapon_debounce"), &NightmareCharacter::end_weapon_debounce);
}

NightmareCharacter::NightmareCharacter()
{
    _cameraArm = nullptr;
    _inputVectorDisplay = nullptr;
    _interactVolume = nullptr;
    _debugText = nullptr;
    _animationTree = nullptr;
    _audioStreamPlayer = nullptr;

    _weaponReady = false;
    _interactDebounce = false;
    _weaponDebounce = false;
}

NightmareCharacter::~NightmareCharacter()
{
}

void NightmareCharacter::_ready()
{
    if (_inEditor)
    {
        return;
    }
    Pawn::_ready();

    _cameraArm = dynamic_cast<CameraArm *>(get_node_internal("CameraArm"));
    _inputVectorDisplay = dynamic_cast<Node3D *>(get_node_or_null("InputVectorDisplay"));
    _interactVolume = dynamic_cast<Area3D *>(get_node_or_null("Mesh/InteractVolume"));
    _debugText = dynamic_cast<RichTextLabel *>(get_node_or_null("DebugText"));
    _animationTree = Object::cast_to<AnimationTree>(get_node_or_null("AnimationTree"));
    _audioStreamPlayer = Object::cast_to<AudioStreamPlayer3D>(get_node_or_null("AudioStreamPlayer3D"));
    _ui = dynamic_cast<NightmareUi *>(get_node_internal("ActiveUI"));

    _bulletScene = ResourceLoader::get_singleton()->load(_bulletScenePath);

    connect("dialog_changed",  Callable(_ui, "set_dialog"));

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

    if (event->is_action_pressed("interact"))
    {
        interact();
        return;
    }

    if (event->is_action_pressed("weapon_ready"))
    {
        ready_weapon();
        return;
    }

    if (event->is_action_released("weapon_ready"))
    {
        release_weapon();
        return;
    }

    if (event->is_action_pressed("weapon_fire"))
    {
        if (_weaponReady)
        {
            fire_weapon();
        }
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

void NightmareCharacter::ready_weapon()
{
    _weaponReady = true;
    _cameraArm->set_focus(true);
    _animationTree->set("parameters/weapon_ready/blend_amount", 1.0f);
}

void NightmareCharacter::release_weapon()
{
    _weaponReady = false;
    _cameraArm->set_focus(false);
    _animationTree->set("parameters/weapon_ready/blend_amount", 0.0f);
}

void NightmareCharacter::fire_weapon()
{
    if (_weaponDebounce)
    {
        return;
    }

    Node3D *bullet = Object::cast_to<Node3D>(_bulletScene->instantiate());
    get_tree()->get_current_scene()->add_child(bullet);
    bullet->set_position(get_position());
    bullet->set_rotation(_pawnMesh->get_rotation());
    _audioStreamPlayer->play();

    _weaponDebounce = true;
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.5f, false);
    timer->connect("timeout", Callable(this, "end_weapon_debounce"));

    const Vector3 startLocation = get_position();
    const Vector3 endLocation = startLocation + _pawnMesh->get_basis().get_column(2) * -50.0f;
    PhysicsDirectSpaceState3D *spaceState = get_world_3d()->get_direct_space_state();
    Ref<PhysicsRayQueryParameters3D> rayQueryParameters = PhysicsRayQueryParameters3D::create(startLocation, endLocation);
    rayQueryParameters->set_exclude(TypedArray<RID>::make(get_rid()));
    Dictionary rayTraceResult = spaceState->intersect_ray(rayQueryParameters);

    if (rayTraceResult.size() == 0)
    {
        return;
    }
    Object *hitObject = rayTraceResult["collider"];
    if (!hitObject->is_class("Enemy"))
    {
        return;
    }
    Enemy *enemy = dynamic_cast<Enemy *>(hitObject);
    enemy->take_damage(1.0f);
}

void NightmareCharacter::end_interact_debounce()
{
    _interactDebounce = false;
}

void NightmareCharacter::end_weapon_debounce()
{
    _weaponDebounce = false;
}

void NightmareCharacter::interact()
{
    if (_interactDebounce)
    {
        return;
    }
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
        UtilityFunctions::print("Interacted with NPC " + hitNpc->get_name());
        hitNpc->trigger_interaction(this);

        _interactDebounce = true;
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1, false);
        timer->connect("timeout", Callable(this, "end_interact_debounce"));
        return;
    }
}

void NightmareCharacter::set_dialog(String dialog)
{
    emit_signal("dialog_changed", dialog);
}

float NightmareCharacter::get_max_speed() const
{
    if (_weaponReady)
    {
        return _readyWalkSpeed;
    }
    return Pawn::get_max_speed();
}