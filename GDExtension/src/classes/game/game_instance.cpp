#include "game_instance.h"

#include "game_state.h"
#include "../level/level.h"
#include "../ui/nightmare_ui.h"
#include "../ui/effects/fade_ui.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GameInstance::GameInstance()
{
    game_state = Ref(memnew(GameState));

    next_scene_path = "";
    next_spawn_location = "";
}

GameInstance::~GameInstance()
{
}

void GameInstance::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("on_fade_out"), &GameInstance::on_fade_out);
    ClassDB::bind_method(D_METHOD("on_fade_back_in"), &GameInstance::on_fade_back_in);
}

void GameInstance::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    game_state->load();
    NightmareUi *nightmare_ui = get_node<NightmareUi>("/root/ActiveUi/");
    FadeUi *fade_effect = nightmare_ui->get_fade_effect();
    if (!fade_effect)
    {
        UtilityFunctions::print("[GameInstance] unable to get UI");
        return;
    }
    fade_effect->connect("fade_out_complete", Callable(this, "on_fade_out"));
    fade_effect->connect("fade_in_complete", Callable(this, "on_fade_back_in"));
}

const Ref<GameState> GameInstance::get_game_state() const
{
    return game_state;
}

void GameInstance::request_level_change(StringName scene_path, String spawn_location)
{
    if (changing_level)
    {
        return;
    }
    UtilityFunctions::print("[GameInstance] level change requested.");
    get_tree()->set_pause(true);

    next_scene_path = scene_path;
    next_spawn_location = spawn_location;

    NightmareUi *nightmare_ui = get_node<NightmareUi>("/root/ActiveUi/");
    FadeUi *fade_effect = nightmare_ui->get_fade_effect();
    if (!fade_effect)
    {
        UtilityFunctions::print("[GameInstance] unable to get UI effect");
        return;
    }
    fade_effect->fade_out(1.0f);
}

void GameInstance::change_level(StringName scene_path, String spawn_location)
{
    UtilityFunctions::print(String("[GameInstance] Loading level scene '{0}'.").format(Array::make(scene_path)));
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
    current_level->set_name(scene_path.get_basename().rsplit("/", false, 1)[1]);
    get_parent()->add_child(current_level);
    get_tree()->set_current_scene(current_level);
    UtilityFunctions::print(String("[GameInstance] Switched level to '{0}'.").format(Array::make(current_level->get_name())) );
}

void GameInstance::on_fade_out()
{
    change_level(next_scene_path, next_spawn_location);

    NightmareUi *nightmare_ui = get_node<NightmareUi>("/root/ActiveUi/");
    FadeUi *fade_effect = nightmare_ui->get_fade_effect();
    if (!fade_effect)
    {
        UtilityFunctions::print("[GameInstance] unable to get UI effect");
        return;
    }
    fade_effect->fade_in(1.0f);
}

void GameInstance::on_fade_back_in()
{
    changing_level = false;
    get_tree()->set_pause(false);
}

NightmareCharacter *GameInstance::get_player() const
{
    Node *current_node = get_tree()->get_current_scene();
    Level *current_level = Object::cast_to<Level>(current_node);
    if (current_level)
    {
        UtilityFunctions::print("[GameInstance] Unable to get player. The current root scene isn't a level.");
        return nullptr;
    }
    
    return current_level->get_player();
}
