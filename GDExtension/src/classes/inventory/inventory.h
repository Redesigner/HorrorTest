#ifndef NIGHTMARE_INVENTORY_H
#define NIGHTMARE_INVENTORY_H

#include <godot_cpp/classes/node.hpp>

#include "../../class_macros.h"

namespace godot
{
class InventoryItemResource;

class Inventory : public Node
{
    GDCLASS(Inventory, Node);

public:
    Inventory();
    ~Inventory();

    void set_inventory(TypedArray<Dictionary> inventory);

    TypedArray<Dictionary> get_inventory() const;

    void add_item(Ref<InventoryItemResource> inventoryResource, int amount);

    bool has_item(Ref<InventoryItemResource> inventoryResource) const;

protected:
    static void _bind_methods();

private:
    TypedArray<Dictionary> _inventory;

    int get_item_index(Ref<InventoryItemResource> inventoryResource) const;
};
}

#endif