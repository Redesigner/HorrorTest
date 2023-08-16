#include "enemy.h"

#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "nightmare_character.h"

using namespace godot;

void Enemy::_bind_methods()
{
    BIND_PROPERTY(Variant::FLOAT, movementSpeed, Enemy);
}

void Enemy::_update_process_callback()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        set_process_mode(PROCESS_MODE_INHERIT);
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

void Enemy::_notification(int p_what)
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

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::_ready()
{
    _navigationAgent = Object::cast_to<NavigationAgent3D>(get_node_or_null("NavigationAgent3D"));
}

void Enemy::_physics_process(double delta)
{
    if (!_navigationAgent)
    {
        UtilityFunctions::print("Unable to find NavigationAgent3D");
        return;
    }

    if (!_target)
    {
        UtilityFunctions::print("No target set for enemy ai");
        if (_target = Object::cast_to<NightmareCharacter>(get_node_or_null("../Player")) )
        {
            _navigationAgent->set_target_position(_target->get_position());
        }
    }
    if (_navigationAgent->is_navigation_finished())
    {
        return;
    }

    Vector3 currentAgentPosition = get_position();
    Vector3 nextPathPosition = _navigationAgent->get_next_path_position();

    Vector3 newVelocity = (nextPathPosition - currentAgentPosition).normalized();
    newVelocity *= _movementSpeed;

    set_velocity(newVelocity);

    move_and_slide();
}