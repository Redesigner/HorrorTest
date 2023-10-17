#include "game_instance.h"

#include "game_state.h"
#include "../level/level.h"

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

#include "godot_cpp/variant/utility_functions.hpp"

using namespace godot;

GameInstance::GameInstance()
{
    game_state = Ref(memnew(GameState));
}

GameInstance::~GameInstance()
{
}

void GameInstance::_bind_methods()
{
}

void GameInstance::_ready()
{
    UtilityFunctions::print("Game instance ready.");
    game_state->load();
}

const Ref<GameState> GameInstance::get_game_state() const
{
    return game_state;
}

void GameInstance::change_level(StringName scene_path, String spawn_location)
{
    Ref<PackedScene> next_level_packed = ResourceLoader::get_singleton()->load(scene_path);
    // @todo load levels asynchronously?
    Node *next_scene = next_level_packed->instantiate();

    Level *next_level = Object::cast_to<Level>(next_scene);
    next_level->set_current_spawn_location_name(spawn_location);

    if (!next_level)
    {
        ERR_PRINT(String("Unable to change level. '{0}' is not a valid level.").format(Array::make(scene_path)));
        return;
    }

    Node *current_level = get_tree()->get_current_scene();
    if (current_level)
    {
        game_state->on_level_exit();
        current_level->queue_free();
    }
    current_level = next_level;
    UtilityFunctions::print(String("[GameInstance] Loading level '{0}'.").format(Array::make(current_level->get_name())));
    get_parent()->add_child(current_level);
    get_tree()->set_current_scene(current_level);
}