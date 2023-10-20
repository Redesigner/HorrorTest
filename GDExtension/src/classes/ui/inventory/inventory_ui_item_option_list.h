#pragma once

#include <godot_cpp/classes/control.hpp>
#include "../../../class_macros.h"

#include <godot_cpp/classes/packed_scene.hpp> // can't forward declare this on MSVC

#include "../../inventory/inventory_item_resource.h"

#include <vector>

namespace godot
{
class VBoxContainer;
class InventoryUiItemOption;
class InventoryUiItemOptionList : public Control
{
    GDCLASS(InventoryUiItemOptionList, Control);

public:
    InventoryUiItemOptionList();
    ~InventoryUiItemOptionList();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    void set_selected_item(Ref<InventoryItemResource> item_resource);

private:
    void create_option(String option_name);

    void select_option(int option_index);


    Ref<InventoryItemResource> currently_selected_item;

    DECLARE_PROPERTY_NODEFAULT(NodePath, list_container_path);
    VBoxContainer *ui_list_container;

    DECLARE_PROPERTY_NODEFAULT(String, option_scene_path);
    Ref<PackedScene> option_scene;

    std::vector<InventoryUiItemOption *> option_ui_list;

    int currently_selected_option_index;
};
}