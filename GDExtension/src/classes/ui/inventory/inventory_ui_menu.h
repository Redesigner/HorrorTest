#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../class_macros.h"

namespace godot
{
class InventoryUiMenu : public Control
{
    GDCLASS(InventoryUiMenu, Control);

public:
    InventoryUiMenu();
    ~InventoryUiMenu();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    void set_inventory(class Inventory *inventory);

    void update();

private:
    class InventoryUiItemDisplay *_itemDisplay;
    class InventoryUiItemPreviewList  *_itemPreviews;

    DECLARE_PROPERTY_NODEFAULT(NodePath, itemDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemPreviewsPath);

    class Inventory *_inventory;

    int _currentlySelectedItemIndex;
};
}