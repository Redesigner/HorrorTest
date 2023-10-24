#pragma once

#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
class Equipment
{
public:
    Equipment();
    ~Equipment();

    virtual void fire(Vector3 direction);

    virtual void ready_weapon();
    virtual void release_weapon();

    bool is_ready() const;

private:
    bool weapon_ready;
};
}