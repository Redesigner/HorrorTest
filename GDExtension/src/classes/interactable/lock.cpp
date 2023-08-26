#include "lock.h"

#include "../inventory/inventory.h"
#include "../inventory/inventory_item_resource.h"
#include "../nightmare_character.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Lock::Lock()
{
    _requiredItem = Ref<InventoryItemResource>();
}

Lock::~Lock()
{
}

void Lock::_bind_methods()
{
    BIND_PROPERTY(Variant::STRING, successDialogText, Lock);
    BIND_PROPERTY(Variant::STRING, failDialogText, Lock);
    BIND_PROPERTY(Variant::OBJECT, requiredItem, Lock);
}

void Lock::trigger_interaction(NightmareCharacter *source)
{
    Inventory *inventory = source->get_inventory();
    if (inventory->has_item(_requiredItem))
    {
        source->set_dialog(_successDialogText);
        return;
    }
    source->set_dialog(_failDialogText);
}

void Lock::set_requiredItem(Ref<InventoryItemResource> requiredItem)
{
    _requiredItem = requiredItem;
}

Ref<InventoryItemResource> Lock::get_requiredItem() const
{
    return _requiredItem;
}