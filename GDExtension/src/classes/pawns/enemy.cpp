#include "enemy.h"

#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>

#include <godot_cpp/core/math.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "nightmare_character.h"
#include "../game/game_instance.h"
#include "../game/game_state.h"

using namespace godot;

Enemy::Enemy()
{
    _navigationAgent = nullptr;
    _target = nullptr;
    _animationTree = nullptr;

    _hasSeenTarget = false;
    _alive = true;
}

Enemy::~Enemy()
{
}

void Enemy::_bind_methods()
{
    BIND_PROPERTY(Variant::FLOAT, maxHealth, Enemy);
    BIND_PROPERTY(Variant::FLOAT, currentHealth, Enemy);
}

void Enemy::_ready()
{
    Pawn::_ready();

    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    _navigationAgent = Object::cast_to<NavigationAgent3D>(get_node_or_null("NavigationAgent3D"));
    _label = Object::cast_to<Label3D>(get_node_or_null("Label3D"));
    // there's only ever one target here -- the player
    _target = dynamic_cast<NightmareCharacter *>(get_node_or_null("../Player"));
    _animationTree = Object::cast_to<AnimationTree>(get_node_or_null("AnimationTree"));

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
}

void Enemy::_physics_process(double delta)
{
    if (_inEditor)
    {
        return;
    }
    if (!_alive)
    {
        return;
    }

    update_target();
    update_navigation();

    Pawn::_physics_process(delta);

    // Don't bother formatting the string if there is nothing there, it could get expensive
    if (!_label->is_visible())
    {
        return;
    }

    String text = String("Velocity: '{0}' m/s").format(Array::make(
        Math::round(get_velocity().length() * 100.0f) / 100.0f
        ));
    text += !_hasSeenTarget ? "\nNo target" : "\nTarget seen";
    text += _navigationAgent->is_navigation_finished() ? "\nNavigation finished" : "\nCurrently navigating";
    text += String("\nDistance to target: '{0}'m").format(Array::make(
        Math::round(_navigationAgent->distance_to_target() * 10.0f) / 10.0f
        ));
    text += String("\nInputvector: X:'{0}', Y:'{1}'").format(Array::make(_inputVector.x, _inputVector.y));
    _label->set_text(text);
}

void Enemy::check_nav_map_ready() 
{
    _navMapReady = true;
    UtilityFunctions::print("[Enemy] Nav map ready");
}

void godot::Enemy::update_navigation()
{
    Vector3 velocity = get_velocity();
    if (!_navMapReady)
    {
        UtilityFunctions::print("[Enemy] NavMap not yet ready, checking again");
        check_nav_map_ready();
        return;
    }
    if (!_hasSeenTarget)
    {
        return;
    }
    if (_navigationAgent->is_navigation_finished())
    {
        _inputVector = Vector2(0.0f, 0.0f);
    }
    const Vector3 targetPosition = _target->get_position();
    const Vector3 position = get_position();
    const Vector3 delta = targetPosition - position;

    const float maxTargetDistance = _navigationAgent->get_target_desired_distance();
    if (delta.length_squared() < maxTargetDistance * maxTargetDistance)
    {
        _navigationAgent->set_target_position(position);
        return;
    }
    _navigationAgent->set_target_position(targetPosition);

    Vector3 nextPathPosition = _navigationAgent->get_next_path_position();
    Vector3 newVelocity = (nextPathPosition - position);
    Vector2 requestedInput = Vector2(newVelocity.x, newVelocity.z).normalized();
    _inputVector = requestedInput;
}

void Enemy::update_target()
{
    // for now, we're active as soon as we see the target, so it doesn't matter if we lose sight of it
    if (_hasSeenTarget)
    {
        return;
    }
    if (!_target)
    {
        UtilityFunctions::print(String("[Enemy] Enemy '{0}' was unable to find player. Check that the player can be found in the level tree.").format(Array::make(get_name())));
        _target = dynamic_cast<NightmareCharacter *>(get_node_or_null("../Player"));
        return;
    }
    // can we see the target?
    PhysicsDirectSpaceState3D *spaceState = get_world_3d()->get_direct_space_state();
    Ref<PhysicsRayQueryParameters3D> rayQueryParameters = PhysicsRayQueryParameters3D::create(get_position(), _target->get_position());
    // ignore both self and target
    rayQueryParameters->set_exclude(TypedArray<RID>::make(get_rid(), _target->get_rid() ));
    Dictionary rayTraceResult = spaceState->intersect_ray(rayQueryParameters);
    if (rayTraceResult.size() == 0)
    {
        _hasSeenTarget = true;
        return;
    }
}

void Enemy::set_maxHealth(float maxHealth)
{
    _maxHealth = Math::max(maxHealth, 0.0f);
    _currentHealth = Math::min(_currentHealth, _maxHealth);
}

float Enemy::get_maxHealth() const
{
    return _maxHealth;
}

void Enemy::set_currentHealth(float currentHealth)
{
    _currentHealth = Math::clamp(currentHealth, 0.0f, _maxHealth);
}

float Enemy::get_currentHealth() const
{
    return _currentHealth;
}

void Enemy::take_damage(float damage)
{
    // Can't take any more damage
    if (!_alive)
    {
        return;
    }
    _currentHealth -= damage;
    if (_currentHealth <= 0.0f)
    {
        _currentHealth = 0.0f;
        die();
    }
    UtilityFunctions::print(String("[Enemy] Enemy taken '{0}' damage, '{1}' health remaining").format(Array::make(damage, _currentHealth)) );
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
    data.encode_float(24, _currentHealth);
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

    _currentHealth = data.decode_float(24);
    if (_currentHealth <= 0.0f)
    {
        _alive = false;
    }
}

void Enemy::die()
{
    UtilityFunctions::print("[Enemy] enemy died");
    _alive = false;
    // this also disables collisions as long as the "disable mode" property is set to remove
    set_process_mode(PROCESS_MODE_DISABLED);
    Object *animationStateObject = _animationTree->get("parameters/playback");
    AnimationNodeStateMachinePlayback *stateMachine = Object::cast_to<AnimationNodeStateMachinePlayback>(animationStateObject);
    stateMachine->travel("death_animation");
    
}