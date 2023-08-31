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
    _itemDisplay = nullptr;
    _itemPreviews = nullptr;

    _inventory = nullptr;

    _currentlySelectedItemIndex = 0;
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
    _itemDisplay = dynamic_cast<InventoryUiItemDisplay *>(get_node_or_null(_itemDisplayPath));
    _itemPreviews = dynamic_cast<InventoryUiItemPreviewList *>(get_node_or_null(_itemPreviewsPath));
}

void InventoryUiMenu::_input(const Ref<InputEvent> &event)
{
    // don't allow any inputs here if it's not visible
    // we don't want the player to accidentally change selection
    // or use an item when in regular gameplay
    if (!is_visible())
    {
        return;
    }

    // Possibly consider using separate actions from the character?
    if (event->is_action_pressed("walk_left"))
    {
        decrease_index();
        return;
    }

    if (event->is_action_pressed("walk_right"))
    {
        increase_index();
        return;
    }
}

void InventoryUiMenu::set_inventory(Inventory *inventory)
{
    _inventory = inventory;
    _itemPreviews->set_inventory(inventory);
    update();
}

void InventoryUiMenu::update()
{
    if (!_inventory)
    {
        UtilityFunctions::push_warning("[InventoryUi] Cannot find inventory instance. Make sure to call set_inventory.");
    }
    UtilityFunctions::print("[InventoryUi] Inventory changed, updating ui elements.");
    if (_inventory->get_inventory().size() <= 0)
    {
        UtilityFunctions::print("[InventoryUi] Inventory instance does not contain any items.");
        return;
    }
    update_currently_selected_item();
    _itemPreviews->update();
}

void InventoryUiMenu::increase_index()
{
    int newIndex = _currentlySelectedItemIndex + 1;
    int inventorySize = _inventory->get_inventory().size();
    
    if (newIndex >= inventorySize)
    {
        newIndex -= inventorySize;
    }
    _currentlySelectedItemIndex = newIndex;
    update_currently_selected_item();
}

void InventoryUiMenu::decrease_index()
{
    int newIndex = _currentlySelectedItemIndex - 1;
    int inventorySize = _inventory->get_inventory().size();
    
    if (newIndex < 0)
    {
        newIndex += inventorySize;
    }
    _currentlySelectedItemIndex = newIndex;
    update_currently_selected_item();
}

void InventoryUiMenu::update_currently_selected_item()
{
    Dictionary inventoryItem = _inventory->get_inventory()[_currentlySelectedItemIndex];
    Ref<InventoryItemResource> currentlySelectedItem = inventoryItem["resource"];
    _itemDisplay->update_inventory_item_resource(currentlySelectedItem);

    _itemPreviews->set_selected_index(_currentlySelectedItemIndex);
}
