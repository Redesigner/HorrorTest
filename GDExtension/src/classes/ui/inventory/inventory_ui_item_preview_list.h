#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

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

public:
    virtual void _ready() override;

    void set_inventory(class Inventory *inventory);

    void update();

private:
    void bind_previews_to_array();

private:
    TypedArray<class InventoryUiItemPreview> _itemPreviews;

    class Inventory *_inventory;

    class Control *_containerNode;

    DECLARE_PROPERTY_NODEFAULT(NodePath, containerNodePath);

};
}