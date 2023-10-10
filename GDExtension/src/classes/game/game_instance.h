#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot
{
class GameState;
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

    void change_level(StringName scene_path);

private:
    Ref<GameState> game_state;
};
}