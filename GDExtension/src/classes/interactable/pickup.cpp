#include "pickup.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>

#include "../pawns/nightmare_character.h"
#include "../inventory/inventory.h"
#include "../inventory/inventory_item_resource.h"

using namespace godot;

Pickup::Pickup()
{
    _pickedUp = false;
    _audioStreamPlayer = nullptr;
}

Pickup::~Pickup()
{   
}

void Pickup::_bind_methods()
{
    BIND_PROPERTY(Variant::OBJECT, itemResource, Pickup);

    ClassDB::bind_method(D_METHOD("destroy_self"), &Pickup::destroy_self);
}

void Pickup::trigger_interaction(NightmareCharacter *source)
{
    // Prevent double pickup in case the item is not deleted fast enough
    if (_pickedUp)
    {
        return;
    }
    _pickedUp = true;
    source->get_inventory()->add_item(_itemResource, _itemAmount);
    String itemName = _itemResource->get_itemName();

    // The item should always have a string name, but if it doesn't for some reason,
    // we don't want to display a partial message
    if (itemName.length() <= 0)
    {
        return;
    }

    const char32_t letter = itemName[0];
    const bool startsWithVowel = (letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || letter == 'u');
    const String displayMessage = "You picked up {0} [color=#00FFFF]{1}[/color]";
    const String formattedDisplayMessage = displayMessage.format(Array::make(startsWithVowel ? "an" : "a", itemName));

    if (_audioStreamPlayer)
    {
        _audioStreamPlayer->play();
    }

    source->set_dialog(formattedDisplayMessage);

    // Wait a few frames before actually destroying the item
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.01, false);
    timer->connect("timeout", Callable(this, "destroy_self"));
    return;
}

void Pickup::_ready()
{
    _audioStreamPlayer = Object::cast_to<AudioStreamPlayer3D>(get_node_or_null("AudioStreamPlayer3D"));
}

void Pickup::set_itemResource(Ref<InventoryItemResource> itemResource)
{
    _itemResource = itemResource;
}

Ref<InventoryItemResource> Pickup::get_itemResource() const
{
    return _itemResource;
}

void godot::Pickup::destroy_self()
{
    queue_free();
}
