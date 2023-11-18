#include "debug_renderer.h"

#include <godot_cpp/classes/mesh.hpp>

#include "debug_mesh.h"

using namespace godot;

DebugRenderer::DebugRenderer()
{
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::_bind_methods()
{
}

void DebugRenderer::draw_mesh(Ref<Mesh> mesh, Transform3D mesh_transform, float lifetime)
{
    MeshInstance3D* mesh_instance = memnew(DebugMesh(lifetime));
    add_child(mesh_instance);
    mesh_instance->set_mesh(mesh);
    mesh_instance->set_transform(mesh_transform);
}
