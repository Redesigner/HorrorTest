#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../inventory/inventory_entry.h"

#include "../../../class_macros.h"

namespace godot
{
class InventoryItemResource;
class Label;
class RichTextLabel;
class TextureRect;
class InventoryUiItemOptionList;

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

    String accept();

    void set_item(InventoryEntry item);

    void set_options(std::vector<String> options);

private:
    Label *_itemNameDisplay;
    RichTextLabel *_itemDescriptionDisplay;
    TextureRect *_itemPictureDisplay;
    InventoryUiItemOptionList *_itemOptionDisplay;

    // Paths are properties so we can freely move the layout around
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemNameDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemDescriptionDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemPictureDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemOptionDisplayPath);
};
}