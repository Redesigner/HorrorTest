#include "player_state.h"

#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void PlayerState::load_from_json_pairs(std::vector<std::pair<String, String>> pairs)
{
    ResourceLoader *resource_loader = ResourceLoader::get_singleton();
    for (std::pair<String, String> pair : pairs)
    {
        if (pair.first == "Equipped")
        {
            current_equipment = resource_loader->load(pair.second);
        }
    }
}

String PlayerState::make_string() const
{
    String pairs = "Player {\n";
    String pair_format = "\t\"{0}\" : \"{1}\"\n";
    if (current_equipment.is_valid())
    {
        pairs += pair_format.format(Array::make("Equipped", current_equipment->get_path()));
    }
    pairs += "}\n";
    return pairs;
}
