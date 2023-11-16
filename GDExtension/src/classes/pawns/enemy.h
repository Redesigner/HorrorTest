#ifndef NIGHTMARE_ENEMY_H
#define NIGHTMARE_ENEMY_H

#include "pawn.h"

#include "class_macros.h"

namespace godot
{
class NavigationAgent3D;
class NightmareCharacter;
class Label3D;
class AnimationTree;
class Timer;
class Resource;

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

    void die();

public:
    void set_maxHealth(float maxHealth);
    float get_maxHealth() const;

    void set_currentHealth(float currentHealth);
    float get_currentHealth() const;

    void take_damage(float damage);

    PackedByteArray make_state_data() const;

    void unpack_state_data(PackedByteArray data);

private:
    void attack();

    bool can_attack();

    void update_debug_label();


    NavigationAgent3D *navigation_agent;

    NightmareCharacter *target;

    Label3D *debug_label;

    AnimationTree *animation_tree;

    Timer *attack_cooldown_timer;

    bool nav_map_ready = false;
    bool has_seen_target;
    bool alive;

    float max_health = 10.0f;
    float current_health = 10.0f;

    DECLARE_PROPERTY(float, attack_cooldown_length, 2.0f);

    DECLARE_PROPERTY_NODEFAULT(Ref<Resource>, attack_hitbox_shape);
};
}

#endif