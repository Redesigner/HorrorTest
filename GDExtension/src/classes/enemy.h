#ifndef NIGHTMARE_ENEMY_H
#define NIGHTMARE_ENEMY_H

#include <godot_cpp/classes/character_body3d.hpp>

#include "class_macros.h"

namespace godot
{
class Enemy : public CharacterBody3D
{
    GDCLASS(Enemy, CharacterBody3D)

protected:
    static void _bind_methods();

    void _update_process_callback();

    void _notification(int p_what);

public:
    Enemy();
    ~Enemy();

public:
    virtual void _ready() override;

    virtual void _physics_process(double delta) override;

private:
    class NavigationAgent3D *_navigationAgent;

    class NightmareCharacter *_target;

    DECLARE_PROPERTY(float, movementSpeed, 4.0f);
};
}

#endif