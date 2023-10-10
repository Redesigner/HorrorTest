#include "game_instance.h"

#include "game_state.h"
#include "../level/level.h"

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/resource_loader.hpp"

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

void GameInstance::change_level(StringName scene_path)
{
    Ref<PackedScene> next_level_packed = ResourceLoader::get_singleton()->load(scene_path);
    Node *next_scene = next_level_packed->instantiate();

    Level *next_level = Object::cast_to<Level>(next_scene);

    if (!next_level)
    {
        ERR_PRINT(String("Unable to change level. '{0}' is not a valid level.").format(Array::make(scene_path)));
        return;
    }

    // Level *current_level = 
}
