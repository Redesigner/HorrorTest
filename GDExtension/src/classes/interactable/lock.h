#pragma once

#include "interactable.h"

#include "../../class_macros.h"

#include <godot_cpp/classes/audio_stream.hpp>

namespace godot
{
class InventoryItemResource;
// class AudioStream;
class NightmareCharacter;
class Lock : public Interactable
{
    GDCLASS(Lock, Interactable);

public:
    Lock();
    ~Lock();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void trigger_interaction(NightmareCharacter *source) override;

    void set_requiredItem(Ref<InventoryItemResource> requiredItem);
    Ref<InventoryItemResource> get_requiredItem() const;

    PackedByteArray make_state_data() const;

private:
    void unpack_state_data(PackedByteArray state_data);

    void unlock(NightmareCharacter *source);

    DECLARE_PROPERTY(String, failDialogText, "");
    DECLARE_PROPERTY_NODEFAULT(Ref<AudioStream>, failSound);

    DECLARE_PROPERTY(String, successDialogText, "");
    DECLARE_PROPERTY_NODEFAULT(Ref<AudioStream>, successSound);

    DECLARE_PROPERTY(bool, locked, true);

    Ref<InventoryItemResource> _requiredItem;

    class AudioStreamPlayer3D *audio_stream_player;
};
}