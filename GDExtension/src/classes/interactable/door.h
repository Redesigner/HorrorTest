#pragma once

#include "interactable.h"

#include "../../class_macros.h"

namespace godot
{
class NightmareCharacter;
class Door : public Interactable
{
    GDCLASS(Door, Interactable);

public:
    Door();
    ~Door();

protected:
    static void _bind_methods();

public:
    virtual void trigger_interaction(NightmareCharacter *source) override;

private:
    DECLARE_PROPERTY(String, next_level_path, "");
    DECLARE_PROPERTY(String, spawn_location, "");
};
}