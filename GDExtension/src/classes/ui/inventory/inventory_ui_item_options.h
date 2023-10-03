#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

namespace godot
{
class InventoryUiItemOptions : public Control
{
    GDCLASS(InventoryUiItemOptions, Control);

public:
    InventoryUiItemOptions();
    ~InventoryUiItemOptions();

protected:
    static void _bind_methods();

public:
    void set_selected_item(Ref<class InventoryItemResource> itemResource);
};
}