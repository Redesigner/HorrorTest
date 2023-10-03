#include "game_instance.h"

#include "game_state.h"

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
}

const Ref<GameState> GameInstance::get_game_state() const
{
    return game_state;
}
