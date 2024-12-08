#include "BoxHitbox.h"

BoxHitbox::BoxHitbox(Vector3D* position, Quaternion* orientation, const Vector3D& position_offset, float half_width, float half_height, float half_depth)
    : HitBoxPrimitive(position, orientation, position_offset),
    _half_width(half_width), _half_height(half_height), _half_depth(half_depth)
{
}

bool BoxHitbox::doCollideWithBox(const BoxHitbox* box_hitbox, std::array<CollisionData, 8>& collision_data)
{
    std::array<Vector3D, 6> other_hitbox_normals;
    std::array<Vector3D, 8> current_hitbox_points;


    return false;
}
