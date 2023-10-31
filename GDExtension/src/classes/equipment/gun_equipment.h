#pragma once

#include "equipment_resource.h"

#include <godot_cpp/classes/audio_stream.hpp>

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
    virtual void load_assets() override;

    virtual void fire(Vector3 direction, NightmareCharacter *owner) override;

private:
    DECLARE_PROPERTY(float, damage, 1.0f);

    DECLARE_PROPERTY_NODEFAULT(String, fire_sound_path);
    Ref<AudioStream> fire_sound;
};
}