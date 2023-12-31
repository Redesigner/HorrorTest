#pragma once

#include <godot_cpp/classes/control.hpp>
#include "../stackable_ui_element.h"
#include "../../inventory/inventory_entry.h"

#include "../../../class_macros.h"

namespace godot
{
class InventoryUiItemDisplay;
class InventoryUiItemPreviewList;
class HealthUi;

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

    void set_items(std::vector<InventoryEntry> items);

    void set_selected_item(InventoryEntry item);

    void set_highlighted_preview(int index);

    void set_options(std::vector<String> options);

    HealthUi *get_health_ui() const;

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

    InventoryUiItemDisplay *item_display;
    DECLARE_PROPERTY_NODEFAULT(NodePath, item_display_path);

    InventoryUiItemPreviewList  *item_previews;
    DECLARE_PROPERTY_NODEFAULT(NodePath, item_previews_path);

    HealthUi *health_ui;
    DECLARE_PROPERTY_NODEFAULT(NodePath, health_ui_path);

    int currently_selected_item_index;

    int start_index;
};
}