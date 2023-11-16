#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../class_macros.h"

namespace godot
{
class Label;

class HealthUi : public Control
{
    GDCLASS(HealthUi, Control);

public:
    HealthUi();
    ~HealthUi();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    void set_health(float health);

private:
    Label *label;
    DECLARE_PROPERTY_NODEFAULT(NodePath, label_path);
};
}