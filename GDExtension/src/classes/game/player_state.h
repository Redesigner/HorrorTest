#pragma once

#include "../equipment/equipment_resource.h"

namespace godot
{
class EquipmentResource;
struct PlayerState
{
    void load_from_json_pairs(std::vector<std::pair<String, String>> pairs);

    String make_string() const;

    Ref<EquipmentResource> current_equipment;
};
}