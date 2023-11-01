#include "gun_equipment.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/world3D.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../pawns/enemy.h"
#include "../pawns/nightmare_character.h"
#include "../inventory/inventory.h"

using namespace godot;

GunEquipment::GunEquipment()
{
}

GunEquipment::~GunEquipment()
{
}

void GunEquipment::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::FLOAT, damage, GunEquipment, PROPERTY_HINT_RANGE, "0,10,0.5");
    BIND_PROPERTY_HINT(Variant::STRING, fire_sound_path, GunEquipment, PROPERTY_HINT_FILE, "");
    BIND_PROPERTY(Variant::OBJECT, ammo_type, GunEquipment);
}

void GunEquipment::load_assets()
{
    if (IN_EDITOR())
    {
        return;
    }

    fire_sound = ResourceLoader::get_singleton()->load(_fire_sound_path);
    if (!fire_sound.is_valid())
    {
        UtilityFunctions::print("[GunEquipment] failed to load fire sound.");
        return;
    }
}

void GunEquipment::fire(Vector3 direction, NightmareCharacter *owner)
{
    if (!_ammo_type.is_valid())
    {
        WARN_PRINT("[GunEquipment] gun ammo type invalid.");
        return;
    }
    if (!try_consume_ammo(owner))
    {
        UtilityFunctions::print(String("[GunEquipment] tried to fire weapon, but had no ammo of type '{0}'").format(Array::make(_ammo_type->get_path())));
        return;
    }
    if (fire_sound.is_valid())
    {
        owner->play_sound_at_location(fire_sound);
    }
    const Vector3 start_location = owner->get_global_position();
    const Vector3 end_location = start_location + direction * -50.0f;
    PhysicsDirectSpaceState3D *space_state = owner->get_world_3d()->get_direct_space_state();
    Ref<PhysicsRayQueryParameters3D> ray_query_parameters = PhysicsRayQueryParameters3D::create(start_location, end_location);
    ray_query_parameters->set_exclude(TypedArray<RID>::make(owner->get_rid()));
    Dictionary rayTraceResult = space_state->intersect_ray(ray_query_parameters);

    if (rayTraceResult.size() == 0)
    {
        UtilityFunctions::print("[GunEquipment] bullet trace did not hit any objects.");
        return;
    }
    Object *hit_object = rayTraceResult["collider"];
    
    Enemy *enemy = Object::cast_to<Enemy>(hit_object);
    if (!enemy)
    {
        Node *node = Object::cast_to<Node>(hit_object);
        if (!node)
        {
            // The dictionary should *always* return at least a node (it should actually be some kind of a collisionshape3d)
            // but, I can't guarantee that, so this warning is an extra guard.
            WARN_PRINT("[GunEquipment] bullet trace hit an invalid object.");
            return;
        }
        UtilityFunctions::print(String("[GunEquipment] bullet trace object '{0}', but it wasn't an enemy.").format(Array::make(node->get_name())));
        return;
    }
    enemy->take_damage(_damage);
}

bool GunEquipment::try_consume_ammo(NightmareCharacter *owner)
{
    Inventory *inventory = owner->get_inventory();
    return inventory->try_consume_item(_ammo_type);
}
