#pragma once

#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
class SpawnLocation : public Node3D
{
    GDCLASS(SpawnLocation, Node3D);

public:
    SpawnLocation();
    ~SpawnLocation();

protected:
    static void _bind_methods();
};
}