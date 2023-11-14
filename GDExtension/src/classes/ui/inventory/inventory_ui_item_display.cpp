#include "inventory_ui_item_display.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../../inventory/inventory_item_resource.h"
#include "../../inventory/inventory_entry.h"

#include "inventory_ui_item_option_list.h"

using namespace godot;

InventoryUiItemDisplay::InventoryUiItemDisplay()
{
    _itemNameDisplay = nullptr;
    _itemDescriptionDisplay = nullptr;
    _itemPictureDisplay = nullptr;
}

InventoryUiItemDisplay::~InventoryUiItemDisplay()
{
}

void InventoryUiItemDisplay::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, itemNameDisplayPath, InventoryUiItemDisplay);
    BIND_PROPERTY(Variant::NODE_PATH, itemDescriptionDisplayPath, InventoryUiItemDisplay);
    BIND_PROPERTY(Variant::NODE_PATH, itemPictureDisplayPath, InventoryUiItemDisplay);
    BIND_PROPERTY(Variant::NODE_PATH, itemOptionDisplayPath, InventoryUiItemDisplay);
}

void InventoryUiItemDisplay::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }
    _itemNameDisplay = get_node<Label>(_itemNameDisplayPath);
    _itemDescriptionDisplay = get_node<RichTextLabel>(_itemDescriptionDisplayPath);
    _itemPictureDisplay = get_node<TextureRect>(_itemPictureDisplayPath);
    _itemOptionDisplay = get_node<InventoryUiItemOptionList>(_itemOptionDisplayPath);
}

void InventoryUiItemDisplay::scroll_up()
{
    if (!_itemOptionDisplay)
    {
        return;
    }
    _itemOptionDisplay->decrement_index();
}

void InventoryUiItemDisplay::scroll_down()
{
    if (!_itemOptionDisplay)
    {
        return;
    }
    _itemOptionDisplay->increment_index();
}

bool InventoryUiItemDisplay::accept()
{
    if (!_itemOptionDisplay)
    {
        return false;
    }
    _itemOptionDisplay->confirm_selection();
    return false;
}

void InventoryUiItemDisplay::set_item(InventoryEntry item)
{
    if (!_itemNameDisplay || !_itemDescriptionDisplay || !_itemPictureDisplay || !_itemOptionDisplay)
    {
        UtilityFunctions::push_error("Unable to find one or more ui elements for InventoryUiItemDisplay. Check that the path is set correctly.");
        return;
    }
    Ref<InventoryItemResource> item_resource = item.item;
    _itemNameDisplay->set_text(item_resource->get_itemName());
    _itemDescriptionDisplay->set_text(item_resource->get_itemDescription());
    _itemPictureDisplay->set_texture(item_resource->get_itemTexture());
    _itemOptionDisplay->set_selected_item(item_resource);
}
