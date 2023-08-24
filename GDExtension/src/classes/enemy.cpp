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

    _navigationAgent = Object::cast_to<NavigationAgent3D>(get_node_or_null("NavigationAgent3D"));
    _label = Object::cast_to<Label3D>(get_node_or_null("Label3D"));
    // there's only ever one target here -- the player
    _target = dynamic_cast<NightmareCharacter *>(get_node_or_null("../Player"));
    _animationTree = Object::cast_to<AnimationTree>(get_node_or_null("AnimationTree"));

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
    UtilityFunctions::print("Nav map ready");
}

void godot::Enemy::update_navigation()
{
    Vector3 velocity = get_velocity();
    if (!_navMapReady)
    {
        UtilityFunctions::print("NavMap not yet ready, checking again");
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
        UtilityFunctions::print(String("Enemy '{0}' was unable to find player. Check that the player can be found in the level tree.")
            .format(Array::make(get_name())));
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
    _currentHealth -= damage;
    if (_currentHealth <= 0.0f)
    {
        _currentHealth = 0.0f;
        die();
    }
    UtilityFunctions::print(String("Enemy taken '{0}' damage, '{1}' health remaining").format(Array::make(damage, _currentHealth)) );
}

void Enemy::die()
{
    UtilityFunctions::print("enemy died");
    _alive = false;
    Object *animationStateObject = _animationTree->get("parameters/playback");
    AnimationNodeStateMachinePlayback *stateMachine = Object::cast_to<AnimationNodeStateMachinePlayback>(animationStateObject);
    stateMachine->travel("death_animation");
    
}