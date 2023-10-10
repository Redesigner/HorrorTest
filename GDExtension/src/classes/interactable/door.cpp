#include "door.h"

#include "../game/game_instance.h"

using namespace godot;

Door::Door()
{
}

Door::~Door()
{
}

void Door::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::STRING, next_level_path, Door, PROPERTY_HINT_FILE, "");
    BIND_PROPERTY(Variant::STRING, spawn_location, Door);
}

void Door::trigger_interaction(NightmareCharacter *source)
{
    GameInstance *game = get_node<GameInstance>("/root/DefaultGameInstance");
    game->change_level(_next_level_path, _spawn_location);
}
