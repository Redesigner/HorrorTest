#include "level.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../pawns/nightmare_character.h"
#include "spawn_location.h"

using namespace godot;

Level::Level()
{
}

Level::~Level()
{
}

void Level::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::STRING, player_scene_path, Level, PROPERTY_HINT_FILE, "");
    BIND_PROPERTY(Variant::STRING, current_spawn_location_name, Level);
}

void Level::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    populate_spawn_locations();

    spawn_player();
}

void Level::populate_spawn_locations()
{
    UtilityFunctions::print("[Level] Searching for valid spawn locations...");
    TypedArray<Node> children = get_children();
    for (int i = 0; i < children.size(); i++)
    {
        Object *object = children[i];
        if (SpawnLocation *spawn_location = Object::cast_to<SpawnLocation>(object))
        {
            spawn_location_map.insert(std::pair<String, SpawnLocation *>(spawn_location->get_name(), spawn_location));
        }
    }
    UtilityFunctions::print(String("[Level] Found {0} valid spawn locations.").format(Array::make(spawn_location_map.size())));
}

void Level::spawn_player()
{
    Ref<PackedScene> player_scene = ResourceLoader::get_singleton()->load(_player_scene_path);
    Node *player_node = player_scene->instantiate();
    NightmareCharacter *player = Object::cast_to<NightmareCharacter>(player_node);
    if (!player)
    {
        ERR_PRINT(String("[Level] Spawning player, but the scene at path '{0}' is not a NightmareCharacter.").format(Array::make(_player_scene_path)));
        return;
    }
    add_child(player);
    
    if (spawn_location_map.size() == 0)
    {
        WARN_PRINT("[Level] No valid spawn locations found, make sure that they are direct children of the level.");
        return;
    }

    if (spawn_location_map.count(_current_spawn_location_name) > 0)
    {
        current_spawn_location = spawn_location_map.at(_current_spawn_location_name);
    }
    else
    {
        current_spawn_location = spawn_location_map.begin()->second;
    }

    player->set_global_position(current_spawn_location->get_global_position());
    player->set_global_rotation(current_spawn_location->get_global_rotation());
}
