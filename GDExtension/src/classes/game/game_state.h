#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

#include <map>

namespace godot
{
class Node;
class Inventory;
class FileAccess;
class Enemy;
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

    void register_enemy(Enemy *enemy);

    void on_level_exit();

    void on_level_enter();

    // Should this return a pointer instead of a ref?
    Inventory * get_inventory();

private:
    StringName convert_node_to_key(Node *node);

    void load_inventory(Ref<FileAccess> file);

    void load_inventory_item_from_string(String reference_string, String amount_string);

    String save_file_path;

    std::map<StringName, PackedByteArray> state_map;

    Ref<Inventory> inventory;

    std::vector<Enemy *> enemies_in_level;
};
}