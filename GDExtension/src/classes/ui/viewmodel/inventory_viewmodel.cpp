#include "inventory_viewmodel.h"

#include "../inventory/inventory_ui_menu.h"
#include "../../inventory/inventory.h"
#include "../../equipment/equipment_resource.h"
#include "../../game/game_instance.h"
#include "../../game/game_state.h"
#include "../../game/player_state.h"
#include "../../pawns/nightmare_character.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

InventoryViewModel::InventoryViewModel()
{
    view = nullptr;
    model = nullptr;

    currently_selected_index = 0;
}

InventoryViewModel::~InventoryViewModel()
{
}

void InventoryViewModel::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("update_model"), &InventoryViewModel::update_model);
    ClassDB::bind_method(D_METHOD("on_view_option_selected"), &InventoryViewModel::on_view_option_selected);
    ClassDB::bind_method(D_METHOD("on_selected_index_increased"), &InventoryViewModel::on_selected_index_increased);
    ClassDB::bind_method(D_METHOD("on_selected_index_decreased"), &InventoryViewModel::on_selected_index_decreased);
}

void InventoryViewModel::bind_view(InventoryUiMenu *menu)
{
    view = menu;
    view->connect("option_selected", Callable(this, "on_view_option_selected"));
    view->connect("inventory_index_increased", Callable(this, "on_selected_index_increased"));
    view->connect("inventory_index_decreased", Callable(this, "on_selected_index_decreased"));
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
    if (items.size() > currently_selected_index)
    {
        update_selected_item(currently_selected_index);
    }
}

void InventoryViewModel::on_view_option_selected(String option)
{
    std::vector<InventoryEntry> items = model->get_inventory_entries();
    if (currently_selected_index >= items.size())
    {
        WARN_PRINT("[InventoryViewModel] option selected for item, but the currently selected item index was out of bounds.");
        return;
    }
    Ref<InventoryItemResource> selected_item = items[currently_selected_index].item;
    if (!selected_item.is_valid())
    {
        WARN_PRINT("[InventoryViewModel] option selected for item, but the currently selected item reference was not valid.");
        return;
    }
    UtilityFunctions::print(String("[InventoryViewModel] option '{0}' selected for item '{1}'").format(Array::make(option, selected_item->get_path())));

    if (option == "Equip")
    {
        if (!view)
        {
            return;
        }
        // view is just our entry point, since it is the only node we have access to
        NightmareCharacter *player = view->get_node<GameInstance>("/root/DefaultGameInstance")->get_player();
        if (!player)
        {
            return;
        }
        EquipmentResource *equipment = Object::cast_to<EquipmentResource>(selected_item.ptr());
        if (!equipment)
        {
            WARN_PRINT("[InventoryViewModel] 'Equip' option selected, but the currently selected item is not a type of equipment.");
            return;
        }
        player->equip(equipment);
        view->set_options(get_options_for_item(items[currently_selected_index]));
        return;
    }

    if (option == "Unequip")
    {
        if (!view)
        {
            return;
        }
        // view is just our entry point, since it is the only node we have access to
        NightmareCharacter *player = view->get_node<GameInstance>("/root/DefaultGameInstance")->get_player();
        if (!player)
        {
            return;
        }
        player->unequip();
        view->set_options(get_options_for_item(items[currently_selected_index]));
        return;
    }
}

void InventoryViewModel::on_selected_index_increased()
{
    if (!model)
    {
        return;
    }
    if (currently_selected_index >= model->get_inventory_entries().size() - 1)
    {
        currently_selected_index = 0;
    }
    else
    {
        currently_selected_index++;
    }

    update_selected_item(currently_selected_index);
}

void InventoryViewModel::on_selected_index_decreased()
{
    if (!model)
    {
        return;
    }
    if (currently_selected_index <= 0)
    {
        currently_selected_index = model->get_inventory_entries().size() - 1;
    }
    else
    {
        currently_selected_index--;
    }
    update_selected_item(currently_selected_index);
}

void InventoryViewModel::update_selected_item(uint8_t index)
{
    if (!view)
    {
        return;
    }
    if (!model)
    {
        return;
    }
    std::vector<InventoryEntry> &entries = model->get_inventory_entries();
    InventoryEntry &entry = entries[index];
    view->set_selected_item(entry);
    view->set_highlighted_preview(index);
    view->set_options(get_options_for_item(entry));
}

std::vector<String> InventoryViewModel::get_options_for_item(InventoryEntry item)
{
    std::vector<String> options = std::vector<String>();
    Ref<InventoryItemResource> item_resource = item.item;
    if (!item_resource.is_valid())
    {
        WARN_PRINT("[InventoryViewModel] failed to generate item options list for inventory. The given item was not valid.");
        return options;
    }
    TypedArray<String> resource_defined_options = item.item->get_options();
    for (int i = 0; i < resource_defined_options.size(); i++)
    {
        options.push_back(resource_defined_options[i]);
    }
    if (EquipmentResource *equipment = Object::cast_to<EquipmentResource>(item.item.ptr()))
    {
        GameInstance *game_instance = view->get_node<GameInstance>("/root/DefaultGameInstance");
        PlayerState &player_state = game_instance->get_game_state()->player_state;
        EquipmentResource *current_equipment = player_state.current_equipment.ptr();

        if (current_equipment == equipment)
        {
            options.push_back("Unequip");
        }
        else
        {
            options.push_back("Equip");
        }
    }
    return options;
}
