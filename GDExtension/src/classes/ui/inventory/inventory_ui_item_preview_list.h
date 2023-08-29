#pragma once

#include <godot_cpp/classes/control.hpp>

namespace godot
{
class InventoryUiItemPreviewList : public Control
{
    GDCLASS(InventoryUiItemPreviewList, Control);

public:
    InventoryUiItemPreviewList();
    ~InventoryUiItemPreviewList();

protected:
    static void _bind_methods();
};
}