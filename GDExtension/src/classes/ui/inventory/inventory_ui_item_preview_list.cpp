#include "inventory_ui_item_preview_list.h"

#include <godot_cpp/classes/control.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "inventory_ui_item_preview.h"

#include "../../inventory/inventory.h"
#include "../../inventory/inventory_item_resource.h"

using namespace godot;

InventoryUiItemPreviewList::InventoryUiItemPreviewList()
{
    _inventory = nullptr;
    _containerNode = nullptr;

    _currentlySelectedItemIndex = 0;
}

InventoryUiItemPreviewList::~InventoryUiItemPreviewList()
{
}

void InventoryUiItemPreviewList::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, containerNodePath, InventoryUiItemPreviewList);

    ClassDB::bind_method(D_METHOD("item_count_changed", "p_item_count", "p_item"), &InventoryUiItemPreviewList::item_count_changed);
}

void InventoryUiItemPreviewList::_ready()
{
    _containerNode = Object::cast_to<Control>(get_node_or_null(_containerNodePath));
    if (!_containerNode)
    {
        UtilityFunctions::print("[InventoryUi] Item preview list could not find a container.");
        return;
    }
    bind_previews_to_array();
}

void InventoryUiItemPreviewList::set_inventory(Inventory *inventory)
{
    _inventory = inventory;
    _inventory->connect("item_count_changed", Callable(this, "item_count_changed"));
}

void InventoryUiItemPreviewList::update()
{
    if (!_inventory)
    {
        return;
    }

    TypedArray<Dictionary> inventoryArray = _inventory->get_inventory_array();
    UtilityFunctions::print( String("[InventoryUi] Attaching inventory items to previews - found {0} items in inventory.").format(Array::make(inventoryArray.size())) );
    for (int i = 0; i < _itemPreviews.size(); i++)
    {
        // Return if we're trying to fill the previews beyond the actual size of the array
        if (i >= inventoryArray.size())
        {
            return;
        }
        Dictionary item = inventoryArray[i];
        InventoryUiItemPreview *itemPreview = dynamic_cast<InventoryUiItemPreview *>(static_cast<Object *>(_itemPreviews[i]));
        itemPreview->set_item_resource(item["resource"]);
    }
}

void InventoryUiItemPreviewList::set_selected_index(int index)
{
    InventoryUiItemPreview *oldPreview = dynamic_cast<InventoryUiItemPreview *>(static_cast<Object *>(_itemPreviews[_currentlySelectedItemIndex]));
    oldPreview->set_selected(false);
    _currentlySelectedItemIndex = index;
    
    InventoryUiItemPreview *newPreview = dynamic_cast<InventoryUiItemPreview *>(static_cast<Object *>(_itemPreviews[index]));
    newPreview->set_selected(true);
}

void InventoryUiItemPreviewList::item_count_changed(int count, Ref<InventoryItemResource> item)
{
    UtilityFunctions::print("Item count changed.");
}

void InventoryUiItemPreviewList::bind_previews_to_array()
{
    if (!_containerNode)
    {
        return;
    }

    TypedArray<Node> children = _containerNode->get_children();

    //UtilityFunctions::print( String("[InventoryUi] Searching for inventory preview slots.. found {0} suitable nodes.").format(Array::make(children.size())) );
    // Parse our nodes and store the relevant ones in our local array
    for (int i = 0; i < children.size(); i++)
    {
        Object *child = children[i];
        if (InventoryUiItemPreview *itemPreview = dynamic_cast<InventoryUiItemPreview *>(child) )
        {
            _itemPreviews.append(itemPreview);
        }
    }
}
