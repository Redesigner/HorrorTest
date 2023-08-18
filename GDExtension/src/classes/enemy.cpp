#include "enemy.h"

#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "nightmare_character.h"

using namespace godot;

void Enemy::_bind_methods()
{
    BIND_PROPERTY(Variant::FLOAT, movementSpeed, Enemy);
}

void Enemy::_notification(int p_what)
{
    switch(p_what)
    {
        case NOTIFICATION_ENTER_TREE:
        {
            ERR_FAIL_COND(!is_inside_tree());
            if (Engine::get_singleton()->is_editor_hint())
            {
                set_process_mode(PROCESS_MODE_DISABLED);
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
        }break;
    }
}

Enemy::Enemy()
{
    _target = nullptr;
}

Enemy::~Enemy()
{

}

void Enemy::_ready()
{
    _navigationAgent = Object::cast_to<NavigationAgent3D>(get_node_or_null("NavigationAgent3D"));
    _label = Object::cast_to<Label3D>(get_node_or_null("Label3D"));
}

void godot::Enemy::_process(double delta)
{
}

void Enemy::_physics_process(double delta)
{
    update_navigation();

    if (!is_on_floor())
    {
        Vector3 velocity = get_velocity();
        velocity += Vector3(0.0f, -9.8f * delta, 0.0f);
        set_velocity(velocity);
    }

    move_and_slide();

    String text = String("Velocity: '{0}' m/s").format(Array::make(get_velocity().length()));
    text += _target == nullptr ? "\nNo target" : "\nTarget: " + _target->get_name();
    text += _navigationAgent->is_navigation_finished() ? "\nNavigation finished" : "\nCurrently navigating";
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
        velocity.x = 0.0f;
        velocity.z = 0.0f;
        set_velocity(velocity);
        return;
    }
    if (_navigationAgent->is_navigation_finished())
    {
        Vector3 velocity = get_velocity();
        velocity.x = 0.0f;
        velocity.z = 0.0f;
        set_velocity(velocity);
    }
    _navigationAgent->set_target_position(_target->get_position());

    Vector3 currentAgentPosition = get_position();
    Vector3 nextPathPosition = _navigationAgent->get_next_path_position();

    Vector3 newVelocity = (nextPathPosition - currentAgentPosition).normalized();
    newVelocity *= _movementSpeed;
    newVelocity.y = velocity.y;
    set_velocity(newVelocity);
}
