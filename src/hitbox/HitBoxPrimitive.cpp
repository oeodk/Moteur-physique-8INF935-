#include "HitBoxPrimitive.h"
#include "BoxHitbox.h"

HitBoxPrimitive::HitBoxPrimitive(Vector3D* position, Quaternion* orientation, const Vector3D& position_offset)
    : _position(position), _orientation(orientation), _position_offset(position_offset)
{
}

bool HitBoxPrimitive::doCollideWith(HitBoxPrimitive* hitbox, std::array<CollisionData, 6>& collision_data)
{
    if (auto box_hitbox = dynamic_cast<BoxHitbox*>(hitbox))
    {
        return doCollideWithBox(box_hitbox, collision_data);
    }
    return false;
}
