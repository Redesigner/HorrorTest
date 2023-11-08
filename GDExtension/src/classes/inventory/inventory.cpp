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
    ClassDB::bind_method(D_METHOD("get_inventory"), &Inventory::get_inventory_array);
    ClassDB::bind_method(D_METHOD("set_inventory", "p_inventory"), &Inventory::set_inventory_array);
    ClassDB::add_property("Inventory", PropertyInfo((Variant::ARRAY), "inventory"), "set_inventory", "get_inventory");
    
    ADD_SIGNAL(MethodInfo("inventory_changed"));
    ADD_SIGNAL(MethodInfo("item_count_changed", PropertyInfo(Variant::INT, "new_count"), PropertyInfo(Variant::OBJECT, "item")));
}

void Inventory::set_inventory_array(TypedArray<Dictionary> inventory)
{
    _inventory = inventory;
}

TypedArray<Dictionary> Inventory::get_inventory_array() const
{
    return _inventory;
}

void Inventory::add_item(Ref<InventoryItemResource> inventory_resource, int amount)
{
    const int item_index = get_item_index(inventory_resource);
    if (item_index >= 0)
    {
        Dictionary inventory_entry = _inventory[item_index];
        int new_amount = static_cast<float>(inventory_entry["amount"]) + amount;
        inventory_entry["amount"] = new_amount;

        // emit_signal("inventory_changed");
        emit_signal("item_count_changed", new_amount, inventory_resource);
        return;
    }
    Dictionary new_item = Dictionary();
    new_item["resource"] = inventory_resource;
    new_item["amount"] = amount;
    _inventory.append(new_item);
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
    return _inventory[item_index]["amount"];
}

bool Inventory::try_consume_item(Ref<InventoryItemResource> inventory_resource)
{
    int item_index = get_item_index(inventory_resource);
    if (item_index < 0)
    {
        return false;
    }
    Dictionary item = _inventory[item_index];
    int item_count = item["amount"];
    if (item_count <= 0)
    {
        return false;
    }
    item_count--;
    item["amount"] = item_count;
    
    Ref<InventoryItemResource> item_resource = item["resource"];
    UtilityFunctions::print(String("[Inventory] consuming item '{0}', remaining count: {1}").format(Array::make(item_resource->get_path(), item_count)));
    emit_signal("item_count_changed", item_count, item_resource);
    return true;
}

int Inventory::get_item_index(Ref<InventoryItemResource> inventory_resource) const
{
    for (int i = 0; i < _inventory.size(); i++)
    {
        Dictionary inventory_entry = _inventory[i];
        Ref<InventoryItemResource> entryInventoryResource = inventory_entry["resource"];
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
        Dictionary inventory_entry = _inventory[i];
        Ref<InventoryItemResource> entry_inventory_resource = inventory_entry["resource"];
        StringName entry_name = entry_inventory_resource->get_path();
        int entry_amount = inventory_entry["amount"];

        inventory_entry_data[0] = entry_name;
        inventory_entry_data[1] = entry_amount;

        data += inventory_entry_format.format(inventory_entry_data);
    }
    data += "}\n";
    // UtilityFunctions::print("[Inventory] converted to string:\n" + data);
    return data;
}

void Inventory::unpack_state_data(PackedByteArray state_data)
{
}
