#include "inventory.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include "inventory_item_resource.h"

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

    ADD_SIGNAL(MethodInfo("inventory_changed"));
}

void Inventory::set_inventory(TypedArray<Dictionary> inventory)
{
    _inventory = inventory;
}

TypedArray<Dictionary> Inventory::get_inventory() const
{
    return _inventory;
}

void Inventory::add_item(Ref<InventoryItemResource> inventoryResource, int amount)
{
    const int itemIndex = get_item_index(inventoryResource);
    if (itemIndex >= 0)
    {
        Dictionary inventoryEntry = _inventory[itemIndex];
        inventoryEntry["amount"] = static_cast<float>(inventoryEntry["amount"]) + amount;

        emit_signal("inventory_changed");
        return;
    }
    Dictionary newItem = Dictionary();
    newItem["resource"] = inventoryResource;
    newItem["amount"] = amount;
    _inventory.append(newItem);
    emit_signal("inventory_changed");
}

bool Inventory::has_item(Ref<InventoryItemResource> inventoryResource) const
{
    const int itemIndex = get_item_index(inventoryResource);
    UtilityFunctions::print(String("[Inventory] Checking for item, item index is '{0}'").format(Array::make(itemIndex)));
    return itemIndex >= 0;
}

int Inventory::get_item_index(Ref<InventoryItemResource> inventoryResource) const
{
    for (int i = 0; i < _inventory.size(); i++)
    {
        Dictionary inventoryEntry = _inventory[i];
        Ref<InventoryItemResource> entryInventoryResource = inventoryEntry["resource"];
        if (entryInventoryResource == inventoryResource)
        {
            return i;
        }
    }
    UtilityFunctions::print("[Inventory] Unable to find item in inventory");
    return -1;
}

String Inventory::make_string_data() const
{
    String data = "Inventory {\n";
    const String inventory_entry_format = "\t\"{0}\" : \"{1}\"\n";
    // e.g. ["res://Objects/Objects/Pickups/item_key.tres" : "1"]
    // indicates the resource path (so we can grab it on load) and number, separated by ':'
    // similar to JSON

    // I don't know how much this will save on reallocation, but ¯\_(ツ)_/¯
    Array inventory_entry_data = Array::make(StringName(), 0U);

    for (int i = 0; i < _inventory.size(); i++)
    {
        Dictionary inventory_entry = _inventory[i];
        Ref<InventoryItemResource> entry_inventory_resource = inventory_entry["resource"];
        StringName entry_name = entry_inventory_resource->get_path();
        int entry_amount = inventory_entry["amount"];

        inventory_entry_data[0] = entry_name;
        inventory_entry_data[1] = entry_amount;

        data += inventory_entry_format.format(inventory_entry_data);
    }
    data += "}";
    // UtilityFunctions::print("[Inventory] converted to string:\n" + data);
    return data;
}

void Inventory::unpack_state_data(PackedByteArray state_data)
{
}
