#ifndef NIGHTMARE_NPC_H
#define NIGHTMARE_NPC_H

#include "interactable.h"

#include "class_macros.h"

namespace godot
{
class NPC : public Interactable
{
    GDCLASS(NPC, Interactable)

protected:
    static void _bind_methods();

public:
    NPC();
    ~NPC();

public:
    virtual void trigger_interaction(class NightmareCharacter* source) override;

private:
    DECLARE_PROPERTY(String, dialogText, "");
};
}

#endif