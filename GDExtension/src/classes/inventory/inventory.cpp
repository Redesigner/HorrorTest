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
    ADD_SIGNAL(MethodInfo("inventory_changed"));
    ADD_SIGNAL(MethodInfo("item_count_changed", PropertyInfo(Variant::INT, "new_count"), PropertyInfo(Variant::OBJECT, "item")));
}

void Inventory::add_item(Ref<InventoryItemResource> inventory_resource, int amount)
{
    InventoryEntry new_item = InventoryEntry(inventory_resource, amount);
    add_item(new_item);
}

void Inventory::add_item(InventoryEntry entry)
{
    const int item_index = get_item_index(entry.item);
    if (item_index >= 0)
    {
        InventoryEntry &inventory_entry = _inventory[item_index];
        inventory_entry.count += entry.count;
        emit_signal("item_count_changed", inventory_entry.count, inventory_entry.item);
        return;
    }
    _inventory.push_back(entry);
    emit_signal("inventory_changed");
}

bool Inventory::has_item(Ref<InventoryItemResource> inventory_resource) const
{
    const int itemIndex = get_item_index(inventory_resource);
    UtilityFunctions::print(String("[Inventory] Checking for item, item index is '{0}'").format(Array::make(itemIndex)));
    return itemIndex >= 0;
}

int Inventory::get_item_count(Ref<InventoryItemResource> inventory_resource) const
{
    int item_index = get_item_index(inventory_resource);
    if (item_index < 0)
    {
        return 0;
    }
    return _inventory[item_index].count;
}

bool Inventory::try_consume_item(Ref<InventoryItemResource> inventory_resource)
{
    int item_index = get_item_index(inventory_resource);
    if (item_index < 0)
    {
        return false;
    }
    InventoryEntry &item = _inventory[item_index];
    if (item.count <= 0)
    {
        return false;
    }
    item.count--;
    
    Ref<InventoryItemResource> item_resource = item.item;
    UtilityFunctions::print(String("[Inventory] consuming item '{0}', remaining count: {1}").format(Array::make(item_resource->get_path(), item.count)));
    emit_signal("item_count_changed", item.count, item_resource);
    return true;
}

int Inventory::get_item_index(Ref<InventoryItemResource> inventory_resource) const
{
    for (int i = 0; i < _inventory.size(); i++)
    {
        InventoryEntry inventory_entry = _inventory[i];
        Ref<InventoryItemResource> entryInventoryResource = inventory_entry.item;
        if (entryInventoryResource == inventory_resource)
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
        InventoryEntry inventory_entry = _inventory[i];
        StringName entry_name = inventory_entry.item->get_path();

        inventory_entry_data[0] = entry_name;
        inventory_entry_data[1] = inventory_entry.count;

        data += inventory_entry_format.format(inventory_entry_data);
    }
    data += "}\n";
    // UtilityFunctions::print("[Inventory] converted to string:\n" + data);
    return data;
}

std::vector<InventoryEntry> godot::Inventory::get_inventory_entries() const
{
    return _inventory;
}