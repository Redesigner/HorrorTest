#pragma once

#include <godot_cpp/classes/control.hpp>
#include "../stackable_ui_element.h"

#include "../../../class_macros.h"

namespace godot
{
class InventoryUiMenu : public Control, public StackableUiElement
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

    // StackableUiElement Interface
    virtual void scroll_left() override;
    virtual void scroll_right() override;

    virtual void scroll_up() override;
    virtual void scroll_down() override;

    virtual bool accept() override;

    virtual void show() override;
    virtual void hide() override;

private:
    void increase_index();

    void decrease_index();

    void update_currently_selected_item();

    class InventoryUiItemDisplay *_item_display;
    class InventoryUiItemPreviewList  *_item_previews;

    DECLARE_PROPERTY_NODEFAULT(NodePath, item_display_path);
    DECLARE_PROPERTY_NODEFAULT(NodePath, item_previews_path);

    class Inventory *_inventory;

    int currently_selected_item_index;
};
}