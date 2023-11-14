#pragma once

#include <godot_cpp/classes/ref.hpp>

#include "inventory_item_resource.h"

namespace godot
{
struct InventoryEntry
{
    InventoryEntry(Ref<InventoryItemResource> item, uint8_t count);

    Ref<InventoryItemResource> item;
    uint8_t count;
};
}