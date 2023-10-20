#pragma once

#include <godot_cpp/classes/resource.hpp>

#include "../../class_macros.h"

namespace godot
{
class Texture2D;

class InventoryItemResource : public Resource
{
    GDCLASS(InventoryItemResource, Resource);

public:
    InventoryItemResource();
    ~InventoryItemResource();

    void set_itemTexture(Ref<Texture2D> itemTexture);
    Ref<Texture2D> get_itemTexture() const;

protected:
    static void _bind_methods();

private:
    Ref<Texture2D> _itemTexture;
    DECLARE_PROPERTY(String, itemName, "");
    DECLARE_PROPERTY(String, itemDescription, "");
    DECLARE_PROPERTY_NODEFAULT(TypedArray<String>, options);
};
}