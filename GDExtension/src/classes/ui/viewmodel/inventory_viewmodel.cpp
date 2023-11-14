#include "inventory_viewmodel.h"

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
}

void InventoryViewModel::update_model()
{
}
