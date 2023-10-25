#pragma once

#include "inventory_item_resource.h"

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

private:
    DECLARE_PROPERTY_NODEFAULT(String, equipment_scene_path);
};
}