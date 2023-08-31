#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

namespace godot
{
class InventoryItemResource;
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

    class TextureRect *_itemPictureDisplay;
    class Control *_itemHighlight;
};
}