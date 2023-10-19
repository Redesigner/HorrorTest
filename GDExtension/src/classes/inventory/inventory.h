#ifndef NIGHTMARE_INVENTORY_H
#define NIGHTMARE_INVENTORY_H

#include <godot_cpp/classes/ref_counted.hpp>

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

    void set_inventory_array(TypedArray<Dictionary> inventory);

    TypedArray<Dictionary> get_inventory_array() const;

    void add_item(Ref<InventoryItemResource> inventory_resource, int amount);

    bool has_item(Ref<InventoryItemResource> inventory_resource) const;

    String make_string_data() const;

protected:
    static void _bind_methods();

private:
    int get_item_index(Ref<InventoryItemResource> inventory_resource) const;


    void unpack_state_data(PackedByteArray state_data);

    TypedArray<Dictionary> _inventory;
};
}

#endif