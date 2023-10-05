#include "game_state.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GameState::GameState()
{
    save_file_path = "user://save_game.dat";
}

GameState::~GameState()
{
}

void GameState::_bind_methods()
{
}

void GameState::update_node_state(Node *node, PackedByteArray data)
{
    StringName key = convert_node_to_key(node);
    state_map.insert_or_assign(key, data);
    UtilityFunctions::print(String("[GameState] Writing gamestate data for '{0}': '{1}'").format(Array::make(key, data)));
}

bool GameState::get_node_state(Node *node, PackedByteArray &data_out)
{
    StringName key = convert_node_to_key(node);
    if (state_map.count(key) == 0) // this is the safest way to check key validity for stl maps
    {
        UtilityFunctions::print(String("[GameState] No state found for {0}").format(Array::make(key)));
        return false;
    }
    data_out = state_map[key];
    return true;
}

void GameState::save()
{
    Ref<FileAccess> file = FileAccess::open(save_file_path, FileAccess::ModeFlags::WRITE);
    UtilityFunctions::print(String("[GameState] Saving state for {0} objects").format(Array::make(state_map.size())));
    for (std::pair<StringName, PackedByteArray> entry : state_map)
    {
        UtilityFunctions::print(String("[GameState} Writing entry '{0}:{1}' to file").format(Array::make(entry.first, entry.second.get_string_from_utf8())));
        file->store_string(String(entry.first) + ":");
        // file->store_buffer(entry.second);
        file->store_string(entry.second.get_string_from_utf8());
        file->store_string("\n");
    }
    UtilityFunctions::print("[GameState] Saving game...");
    file->close();
}

void GameState::load()
{
    String data;
    Ref<FileAccess> file = FileAccess::open(save_file_path, FileAccess::ModeFlags::READ);
    while (file->get_position() < file->get_length())
    {
        String data = file->get_line();
        int split = data.find(":");
        StringName key = data.left(split);
        String data_string = data.right(-split - 1);
        UtilityFunctions::print(String("[GameState] Loading state data, raw info: '{0}'").format(Array::make(data_string)));
        PackedByteArray state = data_string.to_utf8_buffer();
        UtilityFunctions::print(String("[GameState] Loading state from file for node {0}:'{1}'").format(Array::make(key, state)));

        state_map.insert(std::pair<StringName, PackedByteArray>(key, state));
    }
}

StringName GameState::convert_node_to_key(Node *node)
{
    return node->get_path().get_concatenated_names();
}
