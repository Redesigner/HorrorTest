#pragma once

#include "equipment_resource.h"

namespace godot
{
class GunEquipment : public EquipmentResource
{
    GDCLASS(GunEquipment, EquipmentResource);

public:
    GunEquipment();
    ~GunEquipment();

protected:
    static void _bind_methods();

public:
    virtual void fire(Vector3 direction, NightmareCharacter *owner) override;

private:
    DECLARE_PROPERTY(float, damage, 1.0f);
};
}