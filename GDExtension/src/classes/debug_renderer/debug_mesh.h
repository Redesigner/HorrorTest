#pragma once

#include <godot_cpp/classes/mesh_instance3d.hpp>

namespace godot
{
class DebugMesh : public MeshInstance3D
{
    GDCLASS(DebugMesh, MeshInstance3D)

    DebugMesh();
public:
    DebugMesh(float lifetime);
    ~DebugMesh();

protected:
    static void _bind_methods();

public:
    virtual void _process(double delta) override;

private:
    float lifetime_remaining;
};
}