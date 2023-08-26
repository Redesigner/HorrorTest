#ifndef NIGHTMARE_NPC_H
#define NIGHTMARE_NPC_H

#include "interactable.h"

#include "class_macros.h"

namespace godot
{
class NPC : public Interactable
{
    GDCLASS(NPC, Interactable)

public:
    NPC();
    ~NPC();

protected:
    static void _bind_methods();

public:
    virtual void trigger_interaction(class NightmareCharacter* source) override;

    virtual void _ready() override;

private:
    DECLARE_PROPERTY(String, dialogText, "");

    class AudioStreamPlayer3D *_audioStreamPlayer;
};
}

#endif