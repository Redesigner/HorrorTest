#ifndef NIGHTMARE_NIGHTMARE_CHARACTER_H
#define NIGHTMARE_NIGHTMARE_CHARACTER_H

#include "pawn.h"

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/audio_stream.hpp>

#include "class_macros.h"

namespace godot
{
class EquipmentResource;
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

    class Inventory* get_inventory();

    void equip(EquipmentResource *equipment);

    void unequip();

    void play_sound_at_location(Ref<AudioStream> sound);

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

    void end_weapon_debounce();

    bool is_weapon_ready() const;


    class CameraArm *camera_arm;
    class Node3D *input_vector_display;
    class Area3D *interact_volume;
    class RichTextLabel *debug_text;
    class AnimationTree *animation_tree;
    class AudioStreamPlayer3D *audio_stream_player;
    class NightmareUi* ui;
    class Inventory *inventory;

    EquipmentResource *current_equipment;

    Ref<class PackedScene> bullet_scene;

    DECLARE_PROPERTY(float, lookSpeedVertical, 1.0f);
    DECLARE_PROPERTY(float, lookSpeedHorizontal, 1.0f);
    DECLARE_PROPERTY(float, readyWalkSpeed, 1.0f);
    DECLARE_PROPERTY(String, bulletScenePath, "");

    bool interact_debounce;
    bool weapon_debounce;
    bool weapon_ready;
};
}

#endif