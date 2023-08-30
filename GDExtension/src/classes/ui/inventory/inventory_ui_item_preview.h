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

private:
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemPictureDisplayPath);

    class TextureRect *_itemPictureDisplay;
};
}