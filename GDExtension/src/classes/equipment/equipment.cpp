#include "equipment.h"

using namespace godot;

Equipment::Equipment()
{
    weapon_ready = false;
}

Equipment::~Equipment()
{
}

void Equipment::fire(Vector3 direction)
{
}

void Equipment::ready_weapon()
{
}

void Equipment::release_weapon()
{
}

bool Equipment::is_ready() const
{
    return weapon_ready;
}
