#ifndef NIGHTMARE_INVENTORY_H
#define NIGHTMARE_INVENTORY_H

#include <godot_cpp/classes/ref_counted.hpp>

#include <vector>

#include "inventory_entry.h"

#include "../../class_macros.h"

namespace godot
{
class InventoryItemResource;

class Inventory : public RefCounted
{
    GDCLASS(Inventory, RefCounted);

public:
    Inventory();
    ~Inventory();

protected:
    static void _bind_methods();

public:
    void add_item(Ref<InventoryItemResource> inventory_resource, int amount);
    void add_item(InventoryEntry entry);

    bool has_item(Ref<InventoryItemResource> inventory_resource) const;

    int get_item_count(Ref<InventoryItemResource> inventory_resource) const;

    bool try_consume_item(Ref<InventoryItemResource> inventory_resource);

    String make_string_data() const;

    std::vector<InventoryEntry> get_inventory_entries() const;

private:
    int get_item_index(Ref<InventoryItemResource> inventory_resource) const;    

    std::vector<InventoryEntry> _inventory;
};
}

#endif