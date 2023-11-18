#include "game_instance.h"

#include "game_state.h"
#include "../level/level.h"
#include "../ui/nightmare_ui.h"
#include "../ui/effects/fade_ui.h"
#include "../ui/viewmodel/inventory_viewmodel.h"
#include "../ui/viewmodel/health_viewmodel.h"
#include "../ui/inventory/inventory_ui_menu.h"
#include "../inventory/inventory.h"
#include "../pawns/nightmare_character.h"
#include "../debug_renderer/debug_renderer.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GameInstance::GameInstance()
{
    game_state = Ref(memnew(GameState));
    inventory_view_model = Ref(memnew(InventoryViewModel));
    health_view_model = Ref(memnew(HealthViewModel));
    debug_renderer = memnew(DebugRenderer);

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

    ClassDB::bind_method(D_METHOD("on_node_added"), &GameInstance::on_node_added);
}

void GameInstance::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }

    add_child(debug_renderer);

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
    setup_inventory_view_model();

    health_view_model->bind_view(nightmare_ui->get_inventory_menu()->get_health_ui());

    UtilityFunctions::print("[GameInstance] game instance loaded.");
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

    current_player = nullptr;
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

void GameInstance::setup_inventory_view_model()
{
    Inventory *inventory = get_game_state()->get_inventory();
    if (!inventory)
    {
        WARN_PRINT("[GameInstance] failed to setup ui InventoryViewModel. Unable to bind Inventory to ViewModel: inventory was null.");
        return;
    }
    NightmareUi *nightmare_ui = get_node<NightmareUi>("/root/ActiveUi/");
    if (!nightmare_ui)
    {
        WARN_PRINT("[GameInstance] failed to setup ui InventoryViewModel. Unable to get child of NightmareUi: ui was null.");
        return;
    }
    InventoryUiMenu *inventory_ui = nightmare_ui->get_inventory_menu();
    if (!inventory_ui)
    {
        WARN_PRINT("[GameInstance] failed to setup ui InventoryViewModel. Unable to bind model NightmareUi to ViewModel: ui was null.");
        return;
    }
    inventory_view_model->bind_model(inventory);
    inventory_view_model->bind_view(inventory_ui);
    inventory_view_model->update_model(); // manually trigger to initialize state
}

String GameInstance::default_path()
{
    return "/root/DefaultGameInstance";
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

void GameInstance::on_node_added(Node *node)
{
    NightmareCharacter *player = Object::cast_to<NightmareCharacter>(node);
    if (!player)
    {
        return;
    }
    on_player_spawned(player);
}

// Called by level when player is spawned?
// this dependency isn't great, but there seems to be reliablity issues with the node_added signal
void GameInstance::on_player_spawned(NightmareCharacter *player)
{
    current_player = player;
    if (NightmareUi *nightmare_ui = get_node<NightmareUi>("/root/ActiveUi/"))
    {
        player->connect("dialog_changed", Callable(nightmare_ui, "set_dialog"));
        UtilityFunctions::print("[GameInstance] connected ui to player successfully");
        health_view_model->bind_model(player);
    }
    else
    {
        WARN_PRINT("[GameInstance] Attempting to connect dialog ui to player, but dialog ui was nullptr");
    }
}

NightmareCharacter *GameInstance::get_player() const
{
    return current_player;
}

DebugRenderer *GameInstance::get_debug_renderer() const
{
    return debug_renderer;
}
