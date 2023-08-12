#ifndef NIGHTMARE_NPC_H
#define NIGHTMARE_NPC_H

#include <godot_cpp/classes/node3d.hpp>

#include "class_macros.h"

namespace godot
{
class NPC : public Node3D
{
    GDCLASS(NPC, Node3D)

protected:
    static void _bind_methods();

public:
    NPC();
    ~NPC();

public:
    void trigger_interaction(class NightmareCharacter* source);

private:
    DECLARE_PROPERTY(String, dialogText, "");
};
}

#endif