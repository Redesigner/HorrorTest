#include "inventory_viewmodel.h"

#include "../inventory/inventory_ui_menu.h"
#include "../../inventory/inventory.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

InventoryViewModel::InventoryViewModel()
{
    view = nullptr;
    model = nullptr;
}

InventoryViewModel::~InventoryViewModel()
{
}

void InventoryViewModel::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("update_model"), &InventoryViewModel::update_model);
}

void InventoryViewModel::bind_view(InventoryUiMenu *menu)
{
    view = menu;
}

void InventoryViewModel::bind_model(Inventory *inventory)
{
    model = inventory;
    model->connect("inventory_changed", Callable(this, "update_model"));
}

void InventoryViewModel::update_model()
{
    if (!view)
    {
        WARN_PRINT("[InventoryViewModel] Model updated, but View was nullptr. Check that it has been bound.");
        return;
    }
    if (!model)
    {
        WARN_PRINT("[InventoryViewModel] Model updated, but was nullptr. Check that the callbacks are bound correctly.");
        return;
    }
    std::vector<InventoryEntry> items = model->get_inventory_entries();
    view->set_items(items);
    if (items.size() > 0)
    {
        view->set_selected_item(model->get_inventory_entries()[0]);
    }
}
