#ifndef NIGHTMARE_PAWN_H
#define NIGHTMARE_PAWN_H

#include <godot_cpp/classes/character_body3d.hpp>

#include "class_macros.h"

namespace godot
{
class Pawn : public CharacterBody3D
{
    GDCLASS(Pawn, CharacterBody3D)

protected:
    static void _bind_methods();

    void _update_process_callback();
    
    void _notification(int p_what);

public:
    Pawn();
    ~Pawn();

    virtual void _ready() override;

    virtual void _process(double delta) override;

    virtual void _physics_process(double delta) override;


protected:
    void look_at_walk_direction(double delta);

    virtual float get_max_speed() const;

    bool input_pressed() const;

    class MeshInstance3D *_pawnMesh;
    /// The main pawn object in the physical world. This is separate so we can rotate it and still play animations on the pawnmesh
    class Node3D *_body;

private:
    DECLARE_PROPERTY(float, maxWalkSpeed, 4.0f);
    DECLARE_PROPERTY(float, groundAcceleration, 8.0f);
    DECLARE_PROPERTY(float, groundFriction, 16.0f);
    DECLARE_PROPERTY(float, turnSpeed, 90.0f);

protected:
    Vector2 _inputVector;
    bool _inEditor;
};
}

#endif