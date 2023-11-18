#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot
{
class MeshInstance3D;
class Mesh;

class DebugRenderer : public Node
{
    GDCLASS(DebugRenderer, Node);

public:
    DebugRenderer();
    ~DebugRenderer();

protected:
    static void _bind_methods();

public:
    void draw_mesh(Ref<Mesh> mesh, Transform3D mesh_transform, float lifetime = -1.0f);
};
}