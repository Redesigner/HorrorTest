#include "inventory_ui_item_display.h"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../../inventory/inventory_item_resource.h"

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
    BIND_PROPERTY(Variant::OBJECT, itemResource, InventoryUiItemDisplay);
    BIND_PROPERTY(Variant::NODE_PATH, itemNameDisplayPath, InventoryUiItemDisplay);
    BIND_PROPERTY(Variant::NODE_PATH, itemDescriptionDisplayPath, InventoryUiItemDisplay);
    BIND_PROPERTY(Variant::NODE_PATH, itemPictureDisplayPath, InventoryUiItemDisplay);
}

void InventoryUiItemDisplay::_ready()
{
    _itemNameDisplay = Object::cast_to<Label>(get_node_or_null(_itemNameDisplayPath));
    _itemDescriptionDisplay = Object::cast_to<RichTextLabel>(get_node_or_null(_itemDescriptionDisplayPath));
    _itemPictureDisplay = Object::cast_to<TextureRect>(get_node_or_null(_itemPictureDisplayPath));
}

void InventoryUiItemDisplay::update_inventory_item_resource(Ref<InventoryItemResource> itemResource)
{
    _itemResource = itemResource;
    if (!_itemNameDisplay || !_itemDescriptionDisplay || !_itemPictureDisplay)
    {
        UtilityFunctions::push_error("Unable to find one or more ui elements for InventoryUiItemDisplay. Check that the path is set correctly.");
        return;
    }

    _itemNameDisplay->set_text(itemResource->get_itemName());
    _itemDescriptionDisplay->set_text(itemResource->get_itemDescription());
    _itemPictureDisplay->set_texture(itemResource->get_itemTexture());
}
