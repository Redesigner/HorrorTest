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

    virtual void _input(const Ref<InputEvent> &event) override;

    void set_inventory(class Inventory *inventory);

    void update();

private:
    void increase_index();

    void decrease_index();

    void update_currently_selected_item();

    class InventoryUiItemDisplay *_item_display;
    class InventoryUiItemPreviewList  *_item_previews;

    DECLARE_PROPERTY_NODEFAULT(NodePath, itemDisplayPath);
    DECLARE_PROPERTY_NODEFAULT(NodePath, itemPreviewsPath);

    class Inventory *_inventory;

    int currently_selected_item_index;
};
}