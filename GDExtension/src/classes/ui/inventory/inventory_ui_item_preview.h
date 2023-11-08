#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

namespace godot
{
class InventoryItemResource;
class TextureRect;
class Label;
class InventoryUiItemPreview : public Control
{
    GDCLASS(InventoryUiItemPreview, Control);

public:
    InventoryUiItemPreview();
    ~InventoryUiItemPreview();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    void set_item_resource(Ref<InventoryItemResource> itemResource);

    void set_selected(bool selected);

private:
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemPictureDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemHighlightPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemCountDisplayPath);

    TextureRect *_itemPictureDisplay;
    Control *_itemHighlight;
    Label *_itemCountDisplay;
};
}