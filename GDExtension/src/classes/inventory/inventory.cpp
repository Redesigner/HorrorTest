#include "inventory.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::_bind_methods()
{
    BIND_PROPERTY(Variant::ARRAY, inventory, Inventory);
}


void godot::Inventory::set_inventory(TypedArray<Dictionary> inventory)
{
    _inventory = inventory;
}

TypedArray<Dictionary> godot::Inventory::get_inventory() const
{
    return _inventory;
}

void Inventory::add_item(String inventoryResource, int amount)
{
    const int itemIndex = get_item_index(inventoryResource);
    if (itemIndex >= 0)
    {
        Dictionary inventoryEntry = _inventory[itemIndex];
        inventoryEntry["amount"] = static_cast<float>(inventoryEntry["amount"]) + amount;
        return;
    }
    Dictionary newItem = Dictionary();
    newItem["resource"] = inventoryResource;
    newItem["amount"] = amount;
    _inventory.append(newItem);
}

bool godot::Inventory::has_item(String inventoryResource) const
{
    return get_item_index(inventoryResource) >= 0;
}

int Inventory::get_item_index(String inventoryResource) const
{
    for (int i = 0; i < _inventory.size(); i++)
    {
        Dictionary inventoryEntry = _inventory[i];
        String entryInventoryResource = inventoryEntry["resource"];
        if (entryInventoryResource == inventoryResource)
        {
            return i;
        }
    }
    return -1;
}