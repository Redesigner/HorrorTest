#include "npc.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include "../nightmare_character.h"

using namespace godot;

void NPC::_bind_methods()
{
    BIND_PROPERTY(Variant::STRING, dialogText, NPC);
    BIND_VIRTUAL_METHOD(NPC, trigger_interaction);
}

NPC::NPC()
{
}

NPC::~NPC()
{
}

void NPC::trigger_interaction(NightmareCharacter *source)
{
    // UtilityFunctions::print(String("Player interacted with this ({0})").format(Array::make(get_name())));
    source->set_dialog(_dialogText);
}
