#ifndef NIGHTMARE_PICKUP_H
#define NIGHTMARE_PICKUP_H

#include "interactable.h"

#include "../../class_macros.h"

namespace godot
{
class InventoryItemResource;

class Pickup : public Interactable
{
    GDCLASS(Pickup, Interactable);

public:
    Pickup();
    ~Pickup();

protected:
    static void _bind_methods();

public:
    virtual void trigger_interaction(NightmareCharacter *source) override;

    void set_itemResource(Ref<InventoryItemResource> itemResource);
    Ref<InventoryItemResource> get_itemResource() const;

    void destroy_self();

private:
    bool _pickedUp;
   
    DECLARE_PROPERTY(int, itemAmount, 1);
    Ref<InventoryItemResource> _itemResource;
};
}

#endif