#pragma once

#include "viewmodel.h"

namespace godot
{
class InventoryUiMenu;
class Inventory;

class InventoryViewModel : public ViewModel
{
public:
    InventoryViewModel();
    ~InventoryViewModel();

protected:
    static void _bind_methods();

public:
    void bind_view(InventoryUiMenu *menu);
    
    void bind_model(Inventory *inventory);

    void update_model();

private:
    Inventory *model;
    InventoryUiMenu *view;
};
}