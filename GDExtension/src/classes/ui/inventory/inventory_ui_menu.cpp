#include "inventory_ui_menu.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/input_event.hpp>

#include "inventory_ui_item_display.h"
#include "inventory_ui_item_preview_list.h"

#include "../../inventory/inventory.h"
#include "../../inventory/inventory_item_resource.h"

using namespace godot;

InventoryUiMenu::InventoryUiMenu()
{
    _item_display = nullptr;
    _item_previews = nullptr;

    _inventory = nullptr;

    currently_selected_item_index = 0;
}

InventoryUiMenu::~InventoryUiMenu()
{
}

void InventoryUiMenu::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, itemDisplayPath, InventoryUiMenu);
    BIND_PROPERTY(Variant::NODE_PATH, itemPreviewsPath, InventoryUiMenu);
}

void InventoryUiMenu::_ready()
{
    _item_display = dynamic_cast<InventoryUiItemDisplay *>(get_node_or_null(_itemDisplayPath));
    _item_previews = dynamic_cast<InventoryUiItemPreviewList *>(get_node_or_null(_itemPreviewsPath));
}

void InventoryUiMenu::set_inventory(Inventory *inventory)
{
    _inventory = inventory;
    _item_previews->set_inventory(inventory);
    update();
}

void InventoryUiMenu::update()
{
    if (!_inventory)
    {
        WARN_PRINT("[InventoryUi] Cannot find inventory instance. Make sure to call set_inventory.");
    }
    UtilityFunctions::print("[InventoryUi] Inventory changed, updating ui elements.");
    if (_inventory->get_inventory_array().size() <= 0)
    {
        UtilityFunctions::print("[InventoryUi] Inventory instance does not contain any items.");
        return;
    }
    update_currently_selected_item();
    _item_previews->update();
}

void InventoryUiMenu::scroll_left()
{
    decrease_index();
}

void InventoryUiMenu::scroll_right()
{
    increase_index();
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
    int new_index = currently_selected_item_index + 1;
    int inventory_size = _inventory->get_inventory_array().size();
    
    if (new_index >= inventory_size)
    {
        new_index -= inventory_size;
    }
    currently_selected_item_index = new_index;
    update_currently_selected_item();
}

void InventoryUiMenu::decrease_index()
{
    int new_index = currently_selected_item_index - 1;
    int inventorySize = _inventory->get_inventory_array().size();
    
    if (new_index < 0)
    {
        new_index += inventorySize;
    }
    currently_selected_item_index = new_index;
    update_currently_selected_item();
}

void InventoryUiMenu::update_currently_selected_item()
{
    TypedArray<Dictionary> inventory_dictionary = _inventory->get_inventory_array();
    if (currently_selected_item_index >= inventory_dictionary.size() || currently_selected_item_index < 0)
    {
        UtilityFunctions::print("[InventoryUi] Currently selected item index out of range.");
        return;
    }
    Dictionary inventory_item = inventory_dictionary[currently_selected_item_index];
    Ref<InventoryItemResource> currentlySelectedItem = inventory_item["resource"];
    _item_display->update_inventory_item_resource(currentlySelectedItem);

    _item_previews->set_selected_index(currently_selected_item_index);
}
