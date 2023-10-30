#pragma once

#include "../inventory/inventory_item_resource.h"

namespace godot
{
class EquipmentResource : public InventoryItemResource
{
    GDCLASS(EquipmentResource, InventoryItemResource);

public:
    EquipmentResource();
    ~EquipmentResource();

protected:
    static void _bind_methods();

public:
    virtual void fire(Vector3 direction, class NightmareCharacter *owner);

private:
};
}