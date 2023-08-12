#include "npc.h"

#include "nightmare_character.h"

using namespace godot;

void NPC::_bind_methods()
{
    BIND_PROPERTY(Variant::STRING, dialogText, NPC, PROPERTY_HINT_NONE);
}

NPC::NPC()
{
}

NPC::~NPC()
{
}

void NPC::trigger_interaction(NightmareCharacter *source)
{
    source->set_dialog(_dialogText);
}
