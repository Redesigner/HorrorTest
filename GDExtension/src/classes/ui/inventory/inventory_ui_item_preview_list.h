#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

#include "../../inventory/inventory_item_resource.h"
#include "../../inventory/inventory_entry.h"

namespace godot
{
class InventoryUiItemPreview;
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


    void set_items(std::vector<InventoryEntry> items);

    void set_selected_index(int index);

    void item_count_changed(int count, Ref<InventoryItemResource> item);

private:
    void bind_previews_to_array();

    InventoryUiItemPreview *get_preview_for_item(Ref<InventoryItemResource> item) const;


    std::vector<InventoryUiItemPreview *> item_previews;

    class Inventory *_inventory;

    class Control *_container_node;

    DECLARE_PROPERTY_NODEFAULT(NodePath, containerNodePath);

    int currently_selected_item_index;

};
}