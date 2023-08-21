#include "enemy.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label3d.hpp>

#include <godot_cpp/core/math.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "nightmare_character.h"

using namespace godot;

Enemy::Enemy()
{
    _navigationAgent = nullptr;
    _target = nullptr;
    // _animationPlayer = nullptr;
}

Enemy::~Enemy()
{

}

void Enemy::_ready()
{
    Pawn::_ready();

    _navigationAgent = Object::cast_to<NavigationAgent3D>(get_node_or_null("NavigationAgent3D"));
    _label = Object::cast_to<Label3D>(get_node_or_null("Label3D"));
    // _animationPlayer = Object::cast_to<AnimationPlayer>(get_node_or_null("AnimationPlayer"));
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
    
    update_navigation();

    Pawn::_physics_process(delta);

    String text = String("Velocity: '{0}' m/s").format(Array::make(
        Math::round(get_velocity().length() * 100.0f) / 100.0f
        ));
    text += _target == nullptr ? "\nNo target" : "\nTarget: " + _target->get_name();
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
    if (_target == nullptr)
    {
        UtilityFunctions::print("No target set for enemy ai");
        _target = Object::cast_to<NightmareCharacter>(get_node_or_null("../Player"));
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
