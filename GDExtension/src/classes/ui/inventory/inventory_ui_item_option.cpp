#include "inventory_ui_item_option.h"

#include <godot_cpp/classes/label.hpp>

using namespace godot;

InventoryUiItemOption::InventoryUiItemOption()
{
    identifier = nullptr;
}

InventoryUiItemOption::~InventoryUiItemOption()
{
}

void InventoryUiItemOption::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, identifier_path, InventoryUiItemOption);
    BIND_PROPERTY(Variant::NODE_PATH, label_path, InventoryUiItemOption);
}

void InventoryUiItemOption::_ready()
{
    identifier = get_node<Control>(_identifier_path);
    label = get_node<Label>(_label_path);

    if (!identifier)
    {
        return;
    }
    identifier->set_visible(false);
}

void InventoryUiItemOption::select()
{
    if (!identifier)
    {
        return;
    }
    identifier->set_visible(true);
}

void InventoryUiItemOption::unselect()
{
    if (!identifier)
    {
        return;
    }
    identifier->set_visible(false);
}

void InventoryUiItemOption::set_text(String text)
{
    if (!label)
    {
        return;
    }

    label->set_text(text);
}
