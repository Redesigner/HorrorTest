#include "inventory_ui_item_preview.h"

#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/label.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../../inventory/inventory_item_resource.h"

using namespace godot;

InventoryUiItemPreview::InventoryUiItemPreview()
{
    _itemPictureDisplay = nullptr;
    _itemHighlight = nullptr;
    _itemCountDisplay = nullptr;
}

InventoryUiItemPreview::~InventoryUiItemPreview()
{
}

void InventoryUiItemPreview::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, itemPictureDisplayPath, InventoryUiItemPreview);
    BIND_PROPERTY(Variant::NODE_PATH, itemHighlightPath, InventoryUiItemPreview);
    BIND_PROPERTY(Variant::NODE_PATH, itemCountDisplayPath, InventoryUiItemPreview);
}

void InventoryUiItemPreview::_ready()
{
    _itemPictureDisplay = get_node<TextureRect>(_itemPictureDisplayPath);
    _itemHighlight = get_node<Control>(_itemHighlightPath);
    _itemCountDisplay = get_node<Label>(_itemCountDisplayPath);
}

void InventoryUiItemPreview::set_item_resource(Ref<InventoryItemResource> item)
{
    if (!item.is_valid())
    {
        return;
    }
    item_resource = item;
    if (!_itemPictureDisplay)
    {
        return;
    }
    _itemPictureDisplay->set_texture(item->get_itemTexture());

    if (!_itemCountDisplay)
    {
        return;
    }
    if (item->get_stackable())
    {
        _itemCountDisplay->set_visible(true);
        _itemCountDisplay->set_text(String::num_uint64(0));
    }
    else
    {
        _itemCountDisplay->set_visible(false);
    }
}

Ref<InventoryItemResource> InventoryUiItemPreview::get_item_resource() const
{
    return item_resource;
}

void InventoryUiItemPreview::set_item_count(int count)
{
    if (_itemCountDisplay)
    {
        _itemCountDisplay->set_text(String::num_uint64(count));
    };
}

void InventoryUiItemPreview::set_selected(bool selected)
{
    _itemHighlight->set_visible(selected);

    if (selected)
    {
        UtilityFunctions::print("[InventoryUi] item selected.");
    }
}
