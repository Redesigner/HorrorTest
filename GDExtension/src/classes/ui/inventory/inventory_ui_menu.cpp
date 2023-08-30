#include "inventory_ui_menu.h"

#include <godot_cpp/variant/utility_functions.hpp>

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

void InventoryUiMenu::set_inventory(Inventory *inventory)
{
    _inventory = inventory;
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
    Dictionary inventoryItem = _inventory->get_inventory()[_currentlySelectedItemIndex];
    Ref<InventoryItemResource> currentlySelectedItem = inventoryItem["resource"];
    _itemDisplay->update_inventory_item_resource(currentlySelectedItem);
}
