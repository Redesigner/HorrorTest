#ifndef NIGHTMARE_INVENTORY_H
#define NIGHTMARE_INVENTORY_H

#include <godot_cpp/classes/node.hpp>

#include "../../class_macros.h"

namespace godot
{
class Inventory : public Node
{
    GDCLASS(Inventory, Node);

public:
    Inventory();
    ~Inventory();

    void set_inventory(TypedArray<Dictionary> inventory);

    TypedArray<Dictionary> get_inventory() const;

    void add_item(String inventoryResource, int amount);

    bool has_item(String inventoryResource) const;

protected:
    static void _bind_methods();

private:
    TypedArray<Dictionary> _inventory;

    int get_item_index(String inventoryResource) const;
};
}

#endif