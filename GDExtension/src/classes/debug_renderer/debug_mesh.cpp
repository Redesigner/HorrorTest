#include "debug_mesh.h"

using namespace godot;

DebugMesh::DebugMesh()
{
}

DebugMesh::DebugMesh(float lifetime)
{
    lifetime_remaining = lifetime;
}

DebugMesh::~DebugMesh()
{
}

void DebugMesh::_bind_methods()
{
}

void DebugMesh::_process(double delta)
{
    lifetime_remaining -= delta;
    if (lifetime_remaining <= 0.0f)
    {
        queue_free();
    }
}
