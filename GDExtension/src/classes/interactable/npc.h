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

    virtual void trigger_interaction(class NightmareCharacter* source) override;

protected:
    static void _bind_methods();

private:
    DECLARE_PROPERTY(String, dialogText, "");
};
}

#endif