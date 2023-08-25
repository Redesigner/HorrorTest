#include "npc.h"

#include "nightmare_character.h"
#include "inventory/inventory.h"

using namespace godot;

void NPC::_bind_methods()
{
    BIND_PROPERTY(Variant::STRING, dialogText, NPC);
    BIND_PROPERTY(Variant::BOOL, requireInventoryItem, NPC);
    BIND_PROPERTY(Variant::STRING, requiredInventoryItem, NPC);
}

NPC::NPC()
{
}

NPC::~NPC()
{
}

void NPC::trigger_interaction(NightmareCharacter *source)
{
    if (!_requireInventoryItem)
    {
        source->set_dialog(_dialogText);
        return;
    }

    if (!source->get_inventory()->has_item(_requiredInventoryItem))
    {
        return;
    }
    source->set_dialog(_dialogText);
}
