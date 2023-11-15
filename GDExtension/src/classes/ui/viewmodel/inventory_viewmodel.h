#pragma once

#include "viewmodel.h"

#include "../../inventory/inventory_entry.h"

namespace godot
{
class InventoryUiMenu;
class Inventory;

class InventoryViewModel : public ViewModel
{
    GDCLASS(InventoryViewModel, ViewModel);

public:
    InventoryViewModel();
    ~InventoryViewModel();

protected:
    static void _bind_methods();

public:
    void bind_view(InventoryUiMenu *menu);
    
    void bind_model(Inventory *inventory);

    void update_model();

    void on_view_option_selected(String option);

    void on_selected_index_increased();

    void on_selected_index_decreased();

private:
    void update_selected_item(uint8_t index);

    std::vector<String> get_options_for_item(InventoryEntry item);

    Inventory *model;
    InventoryUiMenu *view;

    uint8_t currently_selected_index;
};
}