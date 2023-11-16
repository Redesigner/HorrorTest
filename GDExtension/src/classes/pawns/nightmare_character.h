#ifndef NIGHTMARE_NIGHTMARE_CHARACTER_H
#define NIGHTMARE_NIGHTMARE_CHARACTER_H

#include "pawn.h"

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/audio_stream.hpp>

#include "class_macros.h"

namespace godot
{
class CameraArm;
class EquipmentResource;
class Area3D;
class RichTextLabel;
class AnimationTree;
class AudioStreamPlayer3D;
class Inventory;
class GameState;

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

    
    float get_current_health() const;
    void add_health(float health);

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


    CameraArm *camera_arm;
    Node3D *input_vector_display;
    Area3D *interact_volume;
    RichTextLabel *debug_text;
    AnimationTree *animation_tree;
    AudioStreamPlayer3D *audio_stream_player;
    Inventory *inventory;

    Ref<GameState> game_state;

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