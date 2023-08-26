#pragma once

#include "interactable.h"

#include "../../class_macros.h"

#include <godot_cpp/classes/audio_stream.hpp>

namespace godot
{
class InventoryItemResource;
// class AudioStream;

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

    virtual void trigger_interaction(class NightmareCharacter *source) override;

    void set_requiredItem(Ref<InventoryItemResource> requiredItem);
    Ref<InventoryItemResource> get_requiredItem() const;

private:
    DECLARE_PROPERTY(String, failDialogText, "");
    DECLARE_PROPERTY_NODEFAULT(Ref<AudioStream>, failSound);

    DECLARE_PROPERTY(String, successDialogText, "");
    DECLARE_PROPERTY_NODEFAULT(Ref<AudioStream>, successSound);

    Ref<InventoryItemResource> _requiredItem;

    class AudioStreamPlayer3D *_audioStreamPlayer;
};
}