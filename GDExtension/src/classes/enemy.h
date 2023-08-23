#ifndef NIGHTMARE_ENEMY_H
#define NIGHTMARE_ENEMY_H

#include "pawn.h"

#include "class_macros.h"

namespace godot
{
class Enemy : public Pawn
{
    GDCLASS(Enemy, Pawn)

public:
    Enemy();
    ~Enemy();

    virtual void _ready() override;

    virtual void _process(double delta) override;

    virtual void _physics_process(double delta) override;

protected:
    static void _bind_methods();

private:
    void check_nav_map_ready();

    void update_navigation();

    void update_target();

private:
    class NavigationAgent3D *_navigationAgent;
    class NightmareCharacter *_target;
    class Label3D *_label;

    bool _navMapReady = false;

    bool _hasSeenTarget;
};
}

#endif