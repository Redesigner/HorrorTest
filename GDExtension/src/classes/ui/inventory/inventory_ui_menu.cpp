#include "inventory_ui_menu.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "inventory_ui_item_display.h"
#include "inventory_ui_item_preview_list.h"

#include "../../inventory/inventory.h"
#include "../../inventory/inventory_item_resource.h"

using namespace godot;

InventoryUiMenu::InventoryUiMenu()
{
    _item_display = nullptr;
    _item_previews = nullptr;

    currently_selected_item_index = 0;
    start_index = 0;
}

InventoryUiMenu::~InventoryUiMenu()
{
}

void InventoryUiMenu::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, item_display_path, InventoryUiMenu);
    BIND_PROPERTY(Variant::NODE_PATH, item_previews_path, InventoryUiMenu);

    ADD_SIGNAL(MethodInfo("inventory_index_increased"));
    ADD_SIGNAL(MethodInfo("inventory_index_decreased"));
    ADD_SIGNAL(MethodInfo("option_selected", PropertyInfo(Variant::STRING, "option")));
}

void InventoryUiMenu::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    _item_display = get_node<InventoryUiItemDisplay>(_item_display_path);
    _item_previews = get_node<InventoryUiItemPreviewList>(_item_previews_path);
}

void InventoryUiMenu::set_items(std::vector<InventoryEntry> items)
{
    _item_previews->set_items(items);
}

void InventoryUiMenu::set_selected_item(InventoryEntry item)
{
    _item_display->set_item(item);
}

void InventoryUiMenu::set_highlighted_preview(int index)
{
    _item_previews->set_selected_index(index);
}

void InventoryUiMenu::set_options(std::vector<String> options)
{
    _item_display->set_options(options);
}

void InventoryUiMenu::scroll_left()
{
    decrease_index();
}

void InventoryUiMenu::scroll_right()
{
    increase_index();
}

void InventoryUiMenu::scroll_up()
{
    if (!_item_display)
    {
        return;
    }
    _item_display->scroll_up();
}

void InventoryUiMenu::scroll_down()
{
    if (!_item_display)
    {
        return;
    }
    _item_display->scroll_down();
}

bool InventoryUiMenu::accept()
{
    if (!_item_display)
    {
        return false;
    }
    String selected_option = _item_display->accept();
    emit_signal("option_selected", selected_option);
    return selected_option.is_empty();
}

void InventoryUiMenu::show()
{
    set_visible(true);
}

void InventoryUiMenu::hide()
{
    set_visible(false);
}

void InventoryUiMenu::increase_index()
{
    emit_signal("inventory_index_increased");
}

void InventoryUiMenu::decrease_index()
{
    emit_signal("inventory_index_decreased");
}