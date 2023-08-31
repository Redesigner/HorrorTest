#include "inventory_ui_item_preview.h"

#include <godot_cpp/classes/texture_rect.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../../inventory/inventory_item_resource.h"

using namespace godot;

InventoryUiItemPreview::InventoryUiItemPreview()
{
    _itemPictureDisplay = nullptr;
    _itemHighlight = nullptr;
}

InventoryUiItemPreview::~InventoryUiItemPreview()
{
}

void InventoryUiItemPreview::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, itemPictureDisplayPath, InventoryUiItemPreview);
    BIND_PROPERTY(Variant::NODE_PATH, itemHighlightPath, InventoryUiItemPreview);
}

void InventoryUiItemPreview::_ready()
{
    _itemPictureDisplay = Object::cast_to<TextureRect>(get_node_or_null(_itemPictureDisplayPath));
    _itemHighlight = Object::cast_to<Control>(get_node_or_null(_itemHighlightPath));
}

void InventoryUiItemPreview::set_item_resource(Ref<InventoryItemResource> itemResource)
{
    //UtilityFunctions::print( String("[InventoryUi] previewing item {0}").format(Array::make(itemResource->get_itemName())) );
    _itemPictureDisplay->set_texture(itemResource->get_itemTexture());
}

void InventoryUiItemPreview::set_selected(bool selected)
{
    _itemHighlight->set_visible(selected);

    if (selected)
    {
        UtilityFunctions::print("[InventoryUi] item selected.");
    }
}
