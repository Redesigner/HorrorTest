#include "game_state.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::_bind_methods()
{
}

template <class Type>
void GameState::update_node_state(Type node)
{
    PackedByteArray node_state_data = node->make_state_data();
    StringName key = convert_node_to_key(node);
    state_map.insert_or_assign(key, node_state_data);
    UtilityFunctions::print(String("[GameState] Writing gamestate data for '{0}': '{1}'").format(Array::make(key, node_state_data)));
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

StringName GameState::convert_node_to_key(Node *node)
{
    return node->get_path().get_concatenated_names();
}
