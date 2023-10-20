#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

namespace godot
{
class InventoryItemResource;

class InventoryUiItemDisplay : public Control
{
    GDCLASS(InventoryUiItemDisplay, Control);

public:
    InventoryUiItemDisplay();
    ~InventoryUiItemDisplay();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    void scroll_up();
    void scroll_down();

    void update_inventory_item_resource(Ref<InventoryItemResource> itemResource);

private:
    class Label *_itemNameDisplay;
    class RichTextLabel *_itemDescriptionDisplay;
    class TextureRect *_itemPictureDisplay;
    class InventoryUiItemOptionList *_itemOptionDisplay;

    DECLARE_PROPERTY_NODEFAULT(Ref<InventoryItemResource>, itemResource);

    // Paths are properties so we can freely move the layout around
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemNameDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemDescriptionDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemPictureDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemOptionDisplayPath);
};
}