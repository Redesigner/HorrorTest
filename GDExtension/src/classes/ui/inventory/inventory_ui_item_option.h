#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

namespace godot
{
class Label;
class InventoryUiItemOption : public Control
{
    GDCLASS(InventoryUiItemOption, Control);

public:
    InventoryUiItemOption();
    ~InventoryUiItemOption();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    void select();

    void unselect();

    void set_text(String text);

private:
    DECLARE_PROPERTY_NODEFAULT(NodePath, identifier_path);
    Control *identifier;

    DECLARE_PROPERTY_NODEFAULT(NodePath, label_path);
    Label *label;
};
}