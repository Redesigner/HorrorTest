#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

#include "../../inventory/inventory_item_resource.h"

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

    void set_selected_index(int index);

    void item_count_changed(int count, Ref<InventoryItemResource> item);

private:
    void bind_previews_to_array();

private:
    TypedArray<class InventoryUiItemPreview> _itemPreviews;

    class Inventory *_inventory;

    class Control *_containerNode;

    DECLARE_PROPERTY_NODEFAULT(NodePath, containerNodePath);

    int _currentlySelectedItemIndex;

};
}