#pragma once

#include "interactable.h"

#include "../../class_macros.h"

namespace godot
{
class InventoryItemResource;

class Lock : public Interactable
{
    GDCLASS(Lock, Interactable);

public:
    Lock();
    ~Lock();

protected:
    static void _bind_methods();

public:
    virtual void trigger_interaction(class NightmareCharacter *source) override;

    void set_requiredItem(Ref<InventoryItemResource> requiredItem);
    Ref<InventoryItemResource> get_requiredItem() const;

private:
    DECLARE_PROPERTY(String, failDialogText, "");
    DECLARE_PROPERTY(String, successDialogText, "");
    Ref<InventoryItemResource> _requiredItem;
};
}