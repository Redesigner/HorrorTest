#include "game_state.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../inventory/inventory.h"
#include "../inventory/inventory_item_resource.h"
#include "../pawns/enemy.h"

using namespace godot;

GameState::GameState()
{
    save_file_path = "user://save_game.dat";
    fallback_save_file_path = "res://Shared Resources/save_game.dat";
    inventory = Ref(memnew(Inventory));
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
        file->store_64(entry.second.size());
        file->store_buffer(entry.second);
        file->store_string("\n");
    }
    file->store_string(inventory->make_string_data());
    file->store_string(player_state.make_string());
    UtilityFunctions::print("[GameState] Saving game...");
    file->close();
}

// @todo: Write unit tests for saving/loading
void GameState::load()
{
    String data ="";
    StringName key = "";
    UtilityFunctions::print("[GameState] Opening save file...");
    Ref<FileAccess> file = FileAccess::open(save_file_path, FileAccess::ModeFlags::READ);

    if (!file.is_valid())
    {
        UtilityFunctions::print("[GameState] No save file found.");
        UtilityFunctions::print("[GameState] Attempting to load fallback save file...");
        file = FileAccess::open(fallback_save_file_path, FileAccess::ModeFlags::READ);
        if (!file.is_valid())
        {
            UtilityFunctions::print("[GameState] Failed to load fallback save file.");
            return;
        }
    }

    String inventory_control_string = "Inventory {";
    String player_control_string = "Player {";
    while (file->get_position() < file->get_length())
    {
        // @todo: make this not hardcoded, it should equal the string added in inventory's make_string_data function
        if (match_control_string(data, inventory_control_string))
        {
            load_inventory(file);
            data = "";
            continue;
        }
        if (match_control_string(data, player_control_string))
        {
            player_state.load_from_json_pairs(load_json_style_pairs(file));
            data = "";
            continue;
        }

        uint8_t next_char = file->get_8();
        if (next_char == '\n')
        {
            // clear data on each newline
            data = "";
            continue;
        }
        if (next_char == ':')
        {
            key = data;
            data = "";
            int64_t packed_byte_arary_size = file->get_64();

            PackedByteArray state = file->get_buffer(packed_byte_arary_size);
            state_map.insert(std::pair<StringName, PackedByteArray>(key, state));
            UtilityFunctions::print(String("[GameState] Loading state from file for node {0}:'{1}'").format(Array::make(key, state)));
            data = "";
            state.clear();
            continue;
        }
        data += next_char;
    }
}

void GameState::register_enemy(Enemy *enemy)
{
    if (!enemy)
    {
        return;
    }
    enemies_in_level.emplace_back(enemy);
}

void GameState::on_level_exit()
{
    // Manually update the state for certain object types that would otherwise be constantly changing
    // e.g., enemies
    // We don't want to udpate the enemy state every tick, so we do it here

    UtilityFunctions::print(String("[GameState] Writing state information for {0} enemies in level.").format(Array::make(enemies_in_level.size())));
    for (Enemy *enemy : enemies_in_level)
    {
        // Check enemy is not null
        if (!enemy)
        {
            UtilityFunctions::print("[GameState] Unable to write state for enemy, it is null.");
            continue;
        }
        UtilityFunctions::print(String("[GameState] State stored for enemy '{0}'.").format(Array::make(convert_node_to_key(enemy))));
        update_node_state(enemy, enemy->make_state_data());
    }
    enemies_in_level.clear();
}

void GameState::on_level_enter()
{
}

Inventory *GameState::get_inventory()
{
    return inventory.ptr();
}

StringName GameState::convert_node_to_key(Node *node)
{
    return node->get_path().get_concatenated_names();
}

void GameState::load_inventory(Ref<FileAccess> file)
{
    for (std::pair<String, String> inventory_pair : load_json_style_pairs(file))
    {
        load_inventory_item_from_string(inventory_pair.first, inventory_pair.second);
    }
}

void GameState::load_inventory_item_from_string(String resource_string, String amount_string)
{
    UtilityFunctions::print(String("[GameState] loading resource from strings '{0}':'{1}'").format(Array::make(resource_string, amount_string)));
    ResourceLoader *resource_loader = ResourceLoader::get_singleton();
    Ref<InventoryItemResource> item_resource = resource_loader->load(resource_string);
    if (!item_resource.is_valid())
    {
        WARN_PRINT(String("[GameState] Attempt to load inventory item resource at '{0}' failed. Check that the file specified exists, and is of type 'InventoryItemResource'.").format(Array::make(resource_string)));
        return;
    }
    const int amount = amount_string.to_int();
    inventory->add_item(item_resource, amount);
}

std::vector<std::pair<String, String>> GameState::load_json_style_pairs(Ref<FileAccess> file)
{
    std::vector<std::pair<String, String>> data = std::vector<std::pair<String, String>>();
    uint8_t current_char = '0';
    String string_buffer = "";
    bool writing_id = false;
    bool writing_value = false;

    bool reference_valid = false;
    bool value_valid = false;

    String id_string = "";
    String value_string = "";
    while (file->get_position() < file->get_length() && current_char != '}') // '}' should close out our entry
    {
        // The file will always open in single byte intervals
        current_char = file->get_8();

        if (current_char == '\"') // quotation marks
        {
            if (writing_id)
            {
                writing_id = false;
                // write one of our two values
                if (writing_value)
                {
                    value_string = string_buffer;
                    value_valid = true;
                }
                else
                {
                    id_string = string_buffer;
                    reference_valid = true;
                }
            }
            else
            {
                // if we haven't been writing strings, clear the buffer for a new string
                writing_id = true;
                string_buffer = "";
            }
            continue;
        }

        if (current_char == ':' && !writing_id) // allow string names to have colons in them
        {
            // switch writing mode
            writing_value = true;
            continue;
        }

        if (current_char == '\n')
        {
            if (value_valid && reference_valid)
            {
                data.push_back(std::pair<String, String>(id_string, value_string));
                value_valid = false;
                reference_valid = false;
                writing_value = false;
            }
            continue;
        }

        if (current_char == '\t')
        {
            continue;
        }

        // this will ignore psuedo-control characters like : or " becacuse of the previous continue statements
        string_buffer += current_char;
    }
    return data;
}

bool GameState::match_control_string(String data, String control_string)
{
    return data.length() >= control_string.length() && data.right(control_string.length()) == control_string;
}
