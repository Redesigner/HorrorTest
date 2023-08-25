#ifndef NIGHTMARE_INTERACTABLE_H
#define NIGHTMARE_INTERACTABLE_H

#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
class Interactable : public Node3D
{
    GDCLASS(Interactable, Node3D);

public:
    Interactable();
    ~Interactable();

    virtual void trigger_interaction(class NightmareCharacter *source);

protected:
    static void _bind_methods();
};
}

#endif