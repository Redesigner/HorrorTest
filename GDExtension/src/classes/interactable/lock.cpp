#include "lock.h"

#include "../inventory/inventory.h"
#include "../inventory/inventory_item_resource.h"
#include "../pawns/nightmare_character.h"

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/audio_stream.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Lock::Lock()
{
    _requiredItem = Ref<InventoryItemResource>();
    _audioStreamPlayer = nullptr;
}

Lock::~Lock()
{
}

void Lock::_bind_methods()
{
    BIND_PROPERTY(Variant::STRING, successDialogText, Lock);
    BIND_PROPERTY(Variant::OBJECT, successSound, Lock);

    BIND_PROPERTY(Variant::STRING, failDialogText, Lock);
    BIND_PROPERTY(Variant::OBJECT, failSound, Lock);
    BIND_PROPERTY(Variant::OBJECT, requiredItem, Lock);
}

void Lock::_ready()
{
    _audioStreamPlayer = Object::cast_to<AudioStreamPlayer3D>(get_node_or_null("AudioStreamPlayer3D"));
}

void Lock::trigger_interaction(NightmareCharacter *source)
{
    Inventory *inventory = source->get_inventory();
    if (inventory->has_item(_requiredItem))
    {
        source->set_dialog(_successDialogText);
        _audioStreamPlayer->set_stream(_successSound);
        _audioStreamPlayer->play();
        return;
    }
    source->set_dialog(_failDialogText);
    _audioStreamPlayer->set_stream(_failSound);
    _audioStreamPlayer->play();
}

void Lock::set_requiredItem(Ref<InventoryItemResource> requiredItem)
{
    _requiredItem = requiredItem;
}

Ref<InventoryItemResource> Lock::get_requiredItem() const
{
    return _requiredItem;
}