#ifndef NIGHTMARE_NIGHTMARE_CHARACTER_H
#define NIGHTMARE_NIGHTMARE_CHARACTER_H

#include "pawn.h"

#include <godot_cpp/classes/input_event.hpp>

#include "class_macros.h"

namespace godot
{
class NightmareCharacter : public Pawn
{
    GDCLASS(NightmareCharacter, Pawn)

protected:
    static void _bind_methods();

    void _update_process_callback();

public:
    NightmareCharacter();
    ~NightmareCharacter();

    virtual void _ready() override;

	virtual void _input(const Ref<InputEvent> &event) override;

    virtual void _process(double delta) override;

    virtual void _physics_process(double delta) override;

    void interact();

    void set_dialog(String dialog);

protected:
    virtual float get_max_speed() const override;


private:
    void rotate_camera(Vector2 input);

    Vector2 get_input_vector() const;

    void update_input();

    void ready_weapon();

    void release_weapon();

    void fire_weapon();

    void end_interact_debounce();

private:
    class Node3D *_cameraArm;
    class Node3D *_inputVectorDisplay;
    class Area3D *_interactVolume;
    class RichTextLabel *_debugText;
    class AnimationTree *_animationTree;
    class AudioStreamPlayer3D *_audioStreamPlayer;
    class NightmareUi* _ui;

    Ref<class PackedScene> _bulletScene;

    DECLARE_PROPERTY(float, lookSpeedVertical, 1.0f);
    DECLARE_PROPERTY(float, lookSpeedHorizontal, 1.0f);
    DECLARE_PROPERTY(float, readyWalkSpeed, 1.0f);
    DECLARE_PROPERTY(String, bulletScenePath, "");

    bool _weaponReady;

    bool _interactDebounce;
};
}

#endif