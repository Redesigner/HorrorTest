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
    void update_node_state(Node *node, PackedByteArray data);

    bool get_node_state(Node *node, PackedByteArray &data_out);

    void save();

    void load();

private:
    StringName convert_node_to_key(Node *node);

    String save_file_path;

    std::map<StringName, PackedByteArray> state_map;
};
}