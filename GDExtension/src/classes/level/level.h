#pragma once

#include <godot_cpp/classes/node.hpp>

#include <map>

#include "../../class_macros.h"

namespace godot
{
class SpawnLocation;
class Level : public Node
{
    GDCLASS(Level, Node);

public:
    Level();
    ~Level();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

private:
    void populate_spawn_locations();

    void spawn_player();

    std::map<String, SpawnLocation *> spawn_location_map;

    DECLARE_PROPERTY_NODEFAULT(StringName, player_scene_path);

    DECLARE_PROPERTY(String, current_spawn_location_name, "None");
    SpawnLocation *current_spawn_location;
};
}