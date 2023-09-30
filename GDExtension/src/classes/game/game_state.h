#pragma once

#include <godot_cpp/classes/object.hpp>

namespace godot
{
class GameState : public Object
{
    GDCLASS(GameState, Object);

public:
    GameState();
    ~GameState();

protected:
    static void _bind_methods();


};
}