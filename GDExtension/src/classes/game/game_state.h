#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

#include <map>

namespace godot
{
class Node;
class GameState : public RefCounted
{
    GDCLASS(GameState, RefCounted);

public:
    GameState();
    ~GameState();

protected:
    static void _bind_methods();

public:
    template <class Type>
    void update_node_state(Type node);

    bool get_node_state(Node *node, PackedByteArray &data_out);

private:
    StringName convert_node_to_key(Node *node);

    std::map<StringName, PackedByteArray> state_map;
};
}