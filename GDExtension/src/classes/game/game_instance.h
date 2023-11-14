#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot
{
class GameState;
class Level;
class NightmareCharacter;
class ViewModel;
class InventoryViewModel;

class GameInstance : public Node
{
    GDCLASS(GameInstance, Node);

public:
    GameInstance();
    ~GameInstance();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    const Ref<GameState> get_game_state() const;

    void request_level_change(StringName scene_path, String spawn_location);

    void on_fade_out();

    void on_fade_back_in();

    NightmareCharacter *get_player() const;

private:
    void change_level(StringName scene_path, String spawn_location);

    void setup_inventory_view_model();


    Ref<InventoryViewModel> inventory_view_model;

    Ref<GameState> game_state;

    bool changing_level = false;

    StringName next_scene_path;

    String next_spawn_location;
};
}