#include "equipment_resource.h"

using namespace godot;

EquipmentResource::EquipmentResource()
{
    _equipment_scene_path = "";
}

EquipmentResource::~EquipmentResource()
{
}

void EquipmentResource::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::STRING, equipment_scene_path, EquipmentResource, PROPERTY_HINT_FILE, "");
}
