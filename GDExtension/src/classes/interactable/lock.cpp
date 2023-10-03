#include "lock.h"

#include "../inventory/inventory.h"
#include "../inventory/inventory_item_resource.h"
#include "../pawns/nightmare_character.h"
#include "../game/game_instance.h"
#include "../game/game_state.h"

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Lock::Lock()
{
    _requiredItem = Ref<InventoryItemResource>();
    audio_stream_player = nullptr;
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

    BIND_PROPERTY(Variant::BOOL, locked, Lock);
}

void Lock::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    audio_stream_player = Object::cast_to<AudioStreamPlayer3D>(get_node_or_null("AudioStreamPlayer3D"));
    GameInstance *game_instance = get_node<GameInstance>("/root/DefaultGameInstance");

    PackedByteArray node_state = PackedByteArray();
    if (game_instance->get_game_state()->get_node_state(this, node_state))
    {
        unpack_state_data(node_state);
    }
}

void Lock::trigger_interaction(NightmareCharacter *source)
{
    if (!_locked)
    {
        return;
    }
    Inventory *inventory = source->get_inventory();
    if (inventory->has_item(_requiredItem))
    {
        unlock(source);
        return;
    }
    source->set_dialog(_failDialogText);
    audio_stream_player->set_stream(_failSound);
    audio_stream_player->play();
}

void Lock::set_requiredItem(Ref<InventoryItemResource> requiredItem)
{
    _requiredItem = requiredItem;
}

Ref<InventoryItemResource> Lock::get_requiredItem() const
{
    return _requiredItem;
}

PackedByteArray Lock::make_state_data() const
{
    PackedByteArray data = PackedByteArray();
    data.encode_u8(0, _locked);
    return data;
}

void Lock::unpack_state_data(PackedByteArray state_data)
{
    _locked = state_data.decode_u8(0);
}

void Lock::unlock(NightmareCharacter *source)
{
    source->set_dialog(_successDialogText);
    audio_stream_player->set_stream(_successSound);
    audio_stream_player->play();
    _locked = false;
}
