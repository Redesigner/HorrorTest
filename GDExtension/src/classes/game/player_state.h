#pragma once

#include "../equipment/equipment_resource.h"

namespace godot
{
class EquipmentResource;
struct PlayerState
{
    PlayerState();

    void load_from_json_pairs(std::vector<std::pair<String, String>> pairs);

    String make_string() const;


    Ref<EquipmentResource> current_equipment;

    float current_health;

    float max_health;

    const String equipment_string = "Equipped";
    const String current_health_string = "CurrentHealth";
    const String max_health_string = "MaxHealth";
};
}