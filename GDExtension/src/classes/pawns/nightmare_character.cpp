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

#include "../../core/custom_math.h"

#include "../interactable/interactable.h"
#include "../ui/nightmare_ui.h"
#include "../camera_arm.h"
#include "enemy.h"
#include "../inventory/inventory.h"
#include "../equipment/equipment_resource.h"
#include "../game/game_instance.h"
#include "../game/game_state.h"
#include "../game/player_state.h"
#include "../equipment/equipment.h"

using namespace godot;

void NightmareCharacter::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::FLOAT, lookSpeedVertical, NightmareCharacter, PROPERTY_HINT_RANGE, "0,30,0.1");
    BIND_PROPERTY_HINT(Variant::FLOAT, lookSpeedHorizontal, NightmareCharacter, PROPERTY_HINT_RANGE, "0,30,0.01");
    BIND_PROPERTY_HINT(Variant::FLOAT, readyWalkSpeed, NightmareCharacter, PROPERTY_HINT_RANGE, "0,10,0.1,suffix:m/s");
    BIND_PROPERTY(Variant::STRING, bulletScenePath, NightmareCharacter);

    ADD_SIGNAL(MethodInfo("dialog_changed", PropertyInfo(Variant::STRING, "dialog")));
    ADD_SIGNAL(MethodInfo("health_changed", PropertyInfo(Variant::FLOAT, "new_health")));
    ADD_SIGNAL(MethodInfo("character_died"));

    // expose this method to the api for the timer to work
    ClassDB::bind_method(D_METHOD("end_interact_debounce"), &NightmareCharacter::end_interact_debounce);
    ClassDB::bind_method(D_METHOD("end_weapon_debounce"), &NightmareCharacter::end_weapon_debounce);
}

NightmareCharacter::NightmareCharacter()
{
    camera_arm = nullptr;
    input_vector_display = nullptr;
    interact_volume = nullptr;
    debug_text = nullptr;
    animation_tree = nullptr;
    audio_stream_player = nullptr;
    inventory = nullptr;

    current_equipment = nullptr;

    interact_debounce = false;
    weapon_debounce = false;
    weapon_ready = false;
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
    camera_arm = dynamic_cast<CameraArm *>(get_node_internal("CameraArm"));
    input_vector_display = dynamic_cast<Node3D *>(get_node_or_null("InputVectorDisplay"));
    interact_volume = dynamic_cast<Area3D *>(get_node_or_null("Body/Mesh/InteractVolume"));
    debug_text = dynamic_cast<RichTextLabel *>(get_node_or_null("DebugText"));
    animation_tree = Object::cast_to<AnimationTree>(get_node_or_null("AnimationTree"));
    audio_stream_player = Object::cast_to<AudioStreamPlayer3D>(get_node_or_null("AudioStreamPlayer3D"));

    game_state = get_node<GameInstance>("/root/DefaultGameInstance")->get_game_state();
    inventory = game_state->get_inventory();

    bullet_scene = ResourceLoader::get_singleton()->load(_bulletScenePath);

    // With instanced players, this should be connected by the game state
    // inventory->connect("inventory_changed", Callable(ui, "update_inventory"));
    // ui->set_inventory(inventory);

    current_equipment = game_state->player_state.current_equipment.ptr();

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
        fire_weapon();
        return;
    }

    // temporary testing action?
    if (event->is_action_pressed("save"))
    {
        get_node<GameInstance>("/root/DefaultGameInstance")->get_game_state()->save();
        inventory->make_string_data();
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
    if (debug_text->is_visible())
    {
        String text = String("Velocity: '{0}' m/s").format(Array::make(velocity.length()));
        if (input_pressed())
        {
            text += "\nInput currently pressed.";
        }
        if (is_on_floor())
        {
            text += "\nCurrently on the floor.";
        }
        debug_text->set_text(text);
    }

    if (get_position().y < -100.0f)
    {
        set_position(Vector3(0.0f, 5.0f, 0.0f));
        set_velocity(Vector3(0.0f, 0.0f, 0.0f));
    }
}

void NightmareCharacter::rotate_camera(Vector2 input)
{
    if (!camera_arm)
    {
        return;
    }
    Vector3 currentRotation = camera_arm->get_rotation_degrees();
    currentRotation.y += input.x * -_lookSpeedHorizontal;
    currentRotation.x += input.y * -_lookSpeedVertical;
    currentRotation.x = Math::clamp(currentRotation.x, -89.9f, 89.9f);
    camera_arm->set_rotation_degrees(currentRotation);
}

Vector2 NightmareCharacter::get_input_vector() const
{
    return Input::get_singleton()->get_vector("walk_left", "walk_right", "walk_forward", "walk_back");
}

void NightmareCharacter::update_input()
{
    Vector2 inputVector = get_input_vector();
    Vector3 cameraRotation = camera_arm->get_global_rotation();
    float cameraYaw = -cameraRotation.y;
    float cos = Math::cos(cameraYaw);
    float sin = Math::sin(cameraYaw);
    _inputVector = Vector2(cos * inputVector.x - sin * inputVector.y, sin * inputVector.x + cos * inputVector.y);

    input_vector_display->set_position(Vector3(_inputVector.x, input_vector_display->get_position().y, _inputVector.y));

    Vector2 cameraInputVector = Input::get_singleton()->get_vector("look_left", "look_right", "look_up", "look_down");
    rotate_camera(cameraInputVector);
}

void NightmareCharacter::ready_weapon()
{
    if (!current_equipment)
    {
        return;
    }
    camera_arm->set_focus(true);
    animation_tree->set("parameters/weapon_ready/blend_amount", 1.0f);
    weapon_ready = true;
    current_equipment->load_assets();
}

void NightmareCharacter::release_weapon()
{
    if (!current_equipment)
    {
        return;
    }
    camera_arm->set_focus(false);
    animation_tree->set("parameters/weapon_ready/blend_amount", 0.0f);
    weapon_ready = false;
}

void NightmareCharacter::fire_weapon()
{
    if (weapon_debounce)
    {
        return;
    }

    if (!is_weapon_ready())
    {
        return;
    }

    current_equipment->fire(_body->get_global_transform().get_basis().get_column(2), this);
    weapon_debounce = true;
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1f, false);
    timer->connect("timeout", Callable(this, "end_weapon_debounce"));
}

void NightmareCharacter::end_interact_debounce()
{
    interact_debounce = false;
}

void NightmareCharacter::end_weapon_debounce()
{
    weapon_debounce = false;
}

bool NightmareCharacter::is_weapon_ready() const
{
    if (!current_equipment)
    {
        return false;
    }
    //return current_equipment->is_ready();
    return weapon_ready;
}

float NightmareCharacter::get_current_health() const
{
    if (!game_state.is_valid())
    {
        return 0.0f;
    }
    return game_state->player_state.current_health;
}

void NightmareCharacter::add_health(float health)
{
    if (!game_state.is_valid())
    {
        return;
    }
    PlayerState &player_state = game_state->player_state;
    const float max_health = player_state.max_health;
    float &current_health = player_state.current_health;
    // we're taking damage, but can't take any more anyways, don't emit signal here
    if (current_health <= 0.0f && health <= 0.0f)
    {
        return;
    }
    if (current_health + health <= 0.0f)
    {
        current_health = 0.0f;
        emit_signal("health_changed", current_health);
        emit_signal("died");
        return;
    }
    // health is already maxed out, don't emit a signal
    if (current_health >= max_health && health > 0.0f)
    {
        return;
    }
    if (current_health + health >= max_health)
    {
        current_health = max_health;
        emit_signal("health_changed", current_health);
        return;
    }
    current_health += health;
    emit_signal("health_changed", current_health);
}

void NightmareCharacter::interact()
{
    if (interact_debounce)
    {
        return;
    }
    
    TypedArray<Area3D> hitVolumes = interact_volume->get_overlapping_areas();
    for(int i = 0; i < hitVolumes.size(); i++)
    {
        Object *volumeObject = hitVolumes[i];
        Area3D *volume = Object::cast_to<Area3D>(volumeObject);
        if (!volume->get_owner()->is_class("Interactable"))
        {
            continue;
        }

        Interactable *interactable = dynamic_cast<Interactable *>(volume->get_owner());
        interactable->trigger_interaction(this);

        interact_debounce = true;
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1, false);
        timer->connect("timeout", Callable(this, "end_interact_debounce"));
        return;
    }
}

void NightmareCharacter::set_dialog(String dialog)
{
    emit_signal("dialog_changed", dialog);
}

Inventory *NightmareCharacter::get_inventory()
{
    return inventory;
}

void NightmareCharacter::equip(EquipmentResource *equipment)
{
    current_equipment = equipment;
    get_node<GameInstance>("/root/DefaultGameInstance")->get_game_state()->player_state.current_equipment = Ref<EquipmentResource>(equipment);
}

void NightmareCharacter::unequip()
{
    current_equipment = nullptr;
    get_node<GameInstance>("/root/DefaultGameInstance")->get_game_state()->player_state.current_equipment = Ref<EquipmentResource>(nullptr);
}

void NightmareCharacter::play_sound_at_location(Ref<AudioStream> sound)
{
    if (!audio_stream_player)
    {
        UtilityFunctions::print("[NightmareCharacter] attempted to play sound, but the audiostreamplayer was null.");
        return;
    }

    if (!sound.is_valid())
    {
        UtilityFunctions::print("[NightmareCharacter] attempted to play sound, but the sound was invalid.");
        return;
    }

    audio_stream_player->set_stream(sound);
    audio_stream_player->play();
}

float NightmareCharacter::get_max_speed() const
{
    if (is_weapon_ready())
    {
        return _readyWalkSpeed;
    }
    return Pawn::get_max_speed();
}