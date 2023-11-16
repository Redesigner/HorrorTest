#include "enemy.h"

#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/timer.hpp>

#include <godot_cpp/core/math.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "nightmare_character.h"
#include "../game/game_instance.h"
#include "../game/game_state.h"

using namespace godot;

Enemy::Enemy()
{
    navigation_agent = nullptr;
    target = nullptr;
    animation_tree = nullptr;

    has_seen_target = false;
    alive = true;

    attack_cooldown_timer = nullptr;
}

Enemy::~Enemy()
{
    attack_cooldown_timer->queue_free();
}

void Enemy::_bind_methods()
{
    BIND_PROPERTY(Variant::FLOAT, maxHealth, Enemy);
    BIND_PROPERTY(Variant::FLOAT, currentHealth, Enemy);

    BIND_PROPERTY_HINT(Variant::FLOAT, attack_cooldown_length, Enemy, PROPERTY_HINT_NONE, "suffix:s");
    BIND_PROPERTY_HINT(Variant::OBJECT, attack_hitbox_shape, Enemy, PROPERTY_HINT_RESOURCE_TYPE, "Shape3D");
}

void Enemy::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }
    Pawn::_ready();

    navigation_agent = get_node<NavigationAgent3D>("NavigationAgent3D");
    debug_label = get_node<Label3D>("Label3D");
    animation_tree = get_node<AnimationTree>("AnimationTree");
    attack_cooldown_timer = memnew(Timer);
    attack_cooldown_timer->set_one_shot(true);
    add_child(attack_cooldown_timer);

    Ref<GameState> game_state = get_node<GameInstance>("/root/DefaultGameInstance")->get_game_state();
    game_state->register_enemy(this);

    PackedByteArray data = PackedByteArray();
    if (game_state->get_node_state(this, data))
    {
        unpack_state_data(data);
    }
}

void godot::Enemy::_process(double delta)
{
    if (can_attack())
    {
        attack();
    }
}

void Enemy::_physics_process(double delta)
{
    if (_inEditor)
    {
        return;
    }
    if (!alive)
    {
        return;
    }

    update_target();
    update_navigation();

    Pawn::_physics_process(delta);

    update_debug_label();
}

void Enemy::check_nav_map_ready() 
{
    nav_map_ready = true;
    UtilityFunctions::print("[Enemy] Nav map ready");
}

void Enemy::update_navigation()
{
    Vector3 velocity = get_velocity();
    if (!nav_map_ready)
    {
        UtilityFunctions::print("[Enemy] NavMap not yet ready, checking again");
        check_nav_map_ready();
        return;
    }
    if (!has_seen_target)
    {
        return;
    }
    if (navigation_agent->is_navigation_finished())
    {
        _inputVector = Vector2(0.0f, 0.0f);
    }
    const Vector3 targetPosition = target->get_position();
    const Vector3 position = get_position();
    const Vector3 delta = targetPosition - position;

    const float maxTargetDistance = navigation_agent->get_target_desired_distance();
    if (delta.length_squared() < maxTargetDistance * maxTargetDistance)
    {
        navigation_agent->set_target_position(position);
        return;
    }
    navigation_agent->set_target_position(targetPosition);

    Vector3 nextPathPosition = navigation_agent->get_next_path_position();
    Vector3 newVelocity = (nextPathPosition - position);
    Vector2 requestedInput = Vector2(newVelocity.x, newVelocity.z).normalized();
    _inputVector = requestedInput;
}

void Enemy::update_target()
{
    // for now, we're active as soon as we see the target, so it doesn't matter if we lose sight of it
    if (has_seen_target)
    {
        return;
    }
    if (!target)
    {
        UtilityFunctions::print(String("[Enemy] Enemy '{0}' was unable to find player. Check that the player can be found in the level tree.").format(Array::make(get_name())));
        target = dynamic_cast<NightmareCharacter *>(get_node_or_null("../Player"));
        return;
    }
    // can we see the target?
    PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
    Ref<PhysicsRayQueryParameters3D> ray_query_parameters = PhysicsRayQueryParameters3D::create(get_position(), target->get_position());
    // ignore both self and target
    ray_query_parameters->set_exclude(TypedArray<RID>::make(get_rid(), target->get_rid() ));
    Dictionary rayTraceResult = space_state->intersect_ray(ray_query_parameters);
    if (rayTraceResult.size() == 0)
    {
        has_seen_target = true;
        return;
    }
}

void Enemy::set_maxHealth(float health)
{
    max_health = Math::max(health, 0.0f);
    current_health = Math::min(current_health, max_health);
}

float Enemy::get_maxHealth() const
{
    return max_health;
}

void Enemy::set_currentHealth(float health)
{
    current_health = Math::clamp(health, 0.0f, max_health);
}

float Enemy::get_currentHealth() const
{
    return current_health;
}

void Enemy::take_damage(float damage)
{
    // Can't take any more damage
    if (!alive)
    {
        return;
    }
    current_health -= damage;
    if (current_health <= 0.0f)
    {
        current_health = 0.0f;
        die();
    }
    UtilityFunctions::print(String("[Enemy] Enemy taken '{0}' damage, '{1}' health remaining").format(Array::make(damage, current_health)) );
}

PackedByteArray Enemy::make_state_data() const
{
    PackedByteArray data = PackedByteArray();
    const int data_size = sizeof(float) * 7;
    data.resize(data_size);

    const Vector3 global_position = get_global_position();
    const Vector3 global_rotation = get_global_rotation();

    UtilityFunctions::print(String("[Enemy] Stored position as '{0}'").format(Array::make(global_position)));

    data.encode_float(0, global_position.x);
    data.encode_float(4, global_position.y);
    data.encode_float(8, global_position.z);
    data.encode_float(12, global_rotation.x);
    data.encode_float(16, global_rotation.y);
    data.encode_float(20, global_rotation.z);
    data.encode_float(24, current_health);
    return data;
}

void Enemy::unpack_state_data(PackedByteArray data)
{
    Vector3 global_position = Vector3();
    Vector3 global_rotation = Vector3();

    global_position.x = data.decode_float(0);
    global_position.y = data.decode_float(4);
    global_position.z = data.decode_float(8);
    global_rotation.x = data.decode_float(12);
    global_rotation.y = data.decode_float(16);
    global_rotation.z = data.decode_float(20);

    UtilityFunctions::print(String("[Enemy] Loaded position as '{0}'").format(Array::make(global_position)));
    set_global_position(global_position);
    set_global_rotation(global_rotation);

    current_health = data.decode_float(24);
    if (current_health <= 0.0f)
    {
        alive = false;
    }
}

void Enemy::attack()
{
    attack_cooldown_timer->start(_attack_cooldown_length);

    PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
    Ref<PhysicsShapeQueryParameters3D> shape_query_parameters = Ref(memnew(PhysicsShapeQueryParameters3D));
    shape_query_parameters->set_transform(get_mesh_transform());
    shape_query_parameters->set_shape(_attack_hitbox_shape);
    shape_query_parameters->set_exclude(TypedArray<RID>::make(get_rid()));

    TypedArray<Dictionary> overlap_results = space_state->intersect_shape(shape_query_parameters);
    if (overlap_results.size() == 0)
    {
        return;
    }

    UtilityFunctions::print(String("[Enemy] Enemy hit {0} objects with attack.").format(Array::make(overlap_results.size())));
    for (int i = 0; i < overlap_results.size(); i++)
    {
        Dictionary result = overlap_results[i];
        Object *hit_object = result["collider"];
        Node3D *hit_node = Object::cast_to<Node3D>(hit_object);
        if (NightmareCharacter *character = Object::cast_to<NightmareCharacter>(hit_object))
        {
            character->add_health(-1.0f);
            // UtilityFunctions::print("[Enemy] Enemy hit player.");
            continue;
        }
        Node *owner = hit_node->get_owner();
        if (NightmareCharacter *character_owner = Object::cast_to<NightmareCharacter>(owner))
        {
            character_owner->add_health(-1.0f);
            // UtilityFunctions::print("[Enemy] Enemy hit player.");
        }
    }
}

bool Enemy::can_attack()
{
    if (!target)
    {
        return false;
    }
    if (!has_seen_target)
    {
        return false;
    }
    if (attack_cooldown_timer->get_time_left() > 0.0f)
    {
        return false;
    }
    const Vector3 target_position = target->get_global_position();
    const Vector3 enemy_position = get_global_position();
    const float distance_to_target = target_position.distance_squared_to(enemy_position);
    const float minimum_distance_to_target = 2.0f;
    if (distance_to_target > minimum_distance_to_target * minimum_distance_to_target)
    {
        return false;
    }
    return true;
}

void Enemy::update_debug_label()
{
    if (!debug_label->is_visible())
    {
        return;
    }

    String text = String("Velocity: '{0}' m/s").format(Array::make(
        Math::round(get_velocity().length() * 100.0f) / 100.0f
        ));
    text += !has_seen_target ? "\nNo target" : "\nTarget seen";
    text += navigation_agent->is_navigation_finished() ? "\nNavigation finished" : "\nCurrently navigating";
    text += String("\nDistance to target: '{0}'m").format(Array::make(
        Math::round(navigation_agent->distance_to_target() * 10.0f) / 10.0f
        ));
    text += String("\nInputvector: X:'{0}', Y:'{1}'").format(Array::make(_inputVector.x, _inputVector.y));
    debug_label->set_text(text);
}

void Enemy::die()
{
    UtilityFunctions::print("[Enemy] enemy died");
    alive = false;
    // this also disables collisions as long as the "disable mode" property is set to remove
    set_process_mode(PROCESS_MODE_DISABLED);
    Object *animation_state_object = animation_tree->get("parameters/playback");
    AnimationNodeStateMachinePlayback *state_machine = Object::cast_to<AnimationNodeStateMachinePlayback>(animation_state_object);
    state_machine->travel("death_animation");   
}