#include "inventory_ui_item_option_list.h"

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "inventory_ui_item_option.h"
#include "../../game/game_instance.h"
#include "../../game/game_state.h"
#include "../../pawns/nightmare_character.h"
#include "../../equipment/equipment_resource.h"

using namespace godot;

InventoryUiItemOptionList::InventoryUiItemOptionList()
{
    ui_list_container = nullptr;
    option_ui_list = std::vector<InventoryUiItemOption *>();
    currently_selected_option_index = 0;
}

InventoryUiItemOptionList::~InventoryUiItemOptionList()
{
}

void InventoryUiItemOptionList::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, list_container_path, InventoryUiItemOptionList);
    BIND_PROPERTY_HINT(Variant::STRING, option_scene_path, InventoryUiItemOptionList, PropertyHint::PROPERTY_HINT_FILE, "");
}

void InventoryUiItemOptionList::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }

    ui_list_container = get_node<VBoxContainer>(_list_container_path);
    option_scene = ResourceLoader::get_singleton()->load(_option_scene_path);
}

void InventoryUiItemOptionList::increment_index()
{
    int new_selection_index = currently_selected_option_index + 1;
    if (new_selection_index >= option_ui_list.size())
    {
        new_selection_index -= option_ui_list.size();
    }
    select_option(new_selection_index);
}

void InventoryUiItemOptionList::decrement_index()
{
    int new_selection_index = currently_selected_option_index - 1;
    if (new_selection_index < 0)
    {
        new_selection_index += option_ui_list.size();
    }
    select_option(new_selection_index);
}

void InventoryUiItemOptionList::set_options(std::vector<String> options)
{
    for (InventoryUiItemOption *option_ui : option_ui_list)
    {
        option_ui->queue_free();
    }
    option_ui_list.clear();
    option_list.clear();
    for (String option : options)
    {
        create_option(option);
    }
    if (options.size() > 0)
    {
        select_option(0);
    }
}

String InventoryUiItemOptionList::confirm_selection()
{
    std::vector<String> options = option_list;
    if (options.size() <= currently_selected_option_index)
    {
        WARN_PRINT("[InventoryUi] selected option index does not exist for the currently selected item.");
        return "";
    }
    String option = options[currently_selected_option_index];

    UtilityFunctions::print(String("[InventoryUi] selected option '{0}'.").format(Array::make(option)));
    return option;
}

void InventoryUiItemOptionList::create_option(String option_name)
{
    if (!ui_list_container)
    {
        WARN_PRINT("[InventoryUi] Failed to create item option, unable to find valid option container.");
        return;
    }

    if (!option_scene.is_valid())
    {
        WARN_PRINT("[InventoryUi] Failed to create item option, option prefab is not valid.");
        return;
    }

    InventoryUiItemOption *new_option = Object::cast_to<InventoryUiItemOption>(option_scene->instantiate());
    if (!new_option)
    {
        WARN_PRINT("[InventoryUi] Failed to create item option, option prefab is the wrong type. Please make sure it is of type 'InventoryUiItemOption'");
        return;
    }

    ui_list_container->add_child(new_option);
    new_option->set_text(option_name);
    new_option->set_v_size_flags(SIZE_EXPAND_FILL);
    option_ui_list.push_back(new_option);
    option_list.push_back(option_name);
}

void InventoryUiItemOptionList::select_option(int option_index)
{
    // Make sure that our old selection index is still valid before deselecting it
    if (currently_selected_option_index >= 0 && currently_selected_option_index < option_ui_list.size())
    {
        option_ui_list[currently_selected_option_index]->unselect();
    }

    // even if we couldn't unselect the old one, try to select a new one
    // this should ensure that we can select one if the list is invalid for some reason,
    // e.g., we created a new list of options
    if (option_index < 0 || option_index >= option_ui_list.size())
    {
        return;
    }
    currently_selected_option_index = option_index;
    option_ui_list[currently_selected_option_index]->select();
}
