#include "health_ui.h"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

HealthUi::HealthUi()
{
}

HealthUi::~HealthUi()
{
}

void HealthUi::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, label_path, HealthUi);
}

void HealthUi::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }

    label = get_node<Label>(_label_path);
}

void HealthUi::set_health(float health)
{
    if (!label)
    {
        return;
    }
    label->set_text("Health: " + String::num(health));
}
