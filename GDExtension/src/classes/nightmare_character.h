#ifndef NIGHTMARE_NIGHTMARE_CHARACTER_H
#define NIGHTMARE_NIGHTMARE_CHARACTER_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "class_macros.h"

namespace godot
{
class NightmareCharacter : public CharacterBody3D
{
    GDCLASS(NightmareCharacter, CharacterBody3D)

protected:
    static void _bind_methods();

    void _update_process_callback();
    
    void _notification(int p_what);

public:
    NightmareCharacter();
    ~NightmareCharacter();

public:
    virtual void _ready() override;

	virtual void _input(const Ref<InputEvent> &event) override;

    virtual void _process(double delta) override;

    virtual void _physics_process(double delta) override;

    void interact();

    void set_dialog(String dialog);


private:
    Vector2 get_input_vector() const;

    void rotate_camera(Vector2 input);

    bool input_pressed() const;

    void update_input();

    void look_at_walk_direction(double delta);

private:
    class Node3D *_cameraArm;
    class Node3D *_inputVectorDisplay;
    class MeshInstance3D *_playerMesh;
    class Area3D *_interactVolume;
    class RichTextLabel *_debugText;

    DECLARE_PROPERTY(float, lookSpeedVertical, 1.0f);
    DECLARE_PROPERTY(float, lookSpeedHorizontal, 1.0f);
    DECLARE_PROPERTY(float, maxWalkSpeed, 4.0f);
    DECLARE_PROPERTY(float, groundAcceleration, 8.0f);
    DECLARE_PROPERTY(float, groundFriction, 16.0f);
    DECLARE_PROPERTY(float, turnSpeed, 90.0f);
    Vector2 _inputVector;
};
}

#endif