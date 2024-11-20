#pragma once
#include "HitBoxPrimitive.h"

class BoxHitbox 
	: public HitBoxPrimitive
{
	float _half_width, _half_height, _half_depth;
public:
	BoxHitbox(Vector3D* position, Quaternion* orientation, const Vector3D& position_offset, float half_width, float half_height, float half_depth);
	bool doCollideWithBox(const class BoxHitbox* box_hitbox, std::array<CollisionData, 6>& collision_data) override;
};

