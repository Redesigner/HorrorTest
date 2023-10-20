#include "inventory_item_resource.h"

#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

InventoryItemResource::InventoryItemResource()
{
    _options = TypedArray<String>();
}

InventoryItemResource::~InventoryItemResource()
{
}

void InventoryItemResource::_bind_methods()
{
    BIND_PROPERTY(Variant::OBJECT, itemTexture, InventoryItemResource);
    BIND_PROPERTY(Variant::STRING, itemName, InventoryItemResource);
    BIND_PROPERTY(Variant::STRING, itemDescription, InventoryItemResource);
    BIND_PROPERTY(Variant::ARRAY, options, InventoryItemResource);
}

void InventoryItemResource::set_itemTexture(Ref<Texture2D> itemTexture)
{
    _itemTexture = itemTexture;
}

Ref<Texture2D> InventoryItemResource::get_itemTexture() const
{
    return _itemTexture;
}