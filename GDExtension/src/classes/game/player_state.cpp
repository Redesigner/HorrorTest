#include "player_state.h"

#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

PlayerState::PlayerState()
{
    current_health = 3.0f;
    max_health = 3.0f;
}

void PlayerState::load_from_json_pairs(std::vector<std::pair<String, String>> pairs)
{
    ResourceLoader *resource_loader = ResourceLoader::get_singleton();
    for (std::pair<String, String> pair : pairs)
    {
        if (pair.first == equipment_string)
        {
            current_equipment = resource_loader->load(pair.second);
            continue;
        }
        if (pair.first == current_health_string)
        {
            current_health = pair.second.to_float();
            continue;
        }
        if (pair.first == max_health_string)
        {
            max_health = pair.second.to_float();
            continue;
        }
    }
}

String PlayerState::make_string() const
{
    String pairs = "Player {\n";
    const String pair_format = "\t\"{0}\" : \"{1}\"\n";
    if (current_equipment.is_valid())
    {
        pairs += pair_format.format(Array::make(equipment_string, current_equipment->get_path()));
    }
    pairs += pair_format.format(Array::make(current_health_string, current_health));
    pairs += pair_format.format(Array::make(max_health_string, max_health));
    pairs += "}\n";
    return pairs;
}
