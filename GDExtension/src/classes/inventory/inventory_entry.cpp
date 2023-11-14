#include "inventory_entry.h"

using namespace godot;

InventoryEntry::InventoryEntry(Ref<InventoryItemResource> item, uint8_t count)
    : item(item), count(count)
{
}