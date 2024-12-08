#pragma once
#define HITBOX_DEBUG
#include "Vector3D.h"
#include "Quaternion.h"
#include "CollisionData.h"

class HitBoxPrimitive
{
protected:
	Vector3D* _position;
	Quaternion* _orientation;

	Vector3D _position_offset;
	virtual bool doCollideWithBox(const class BoxHitbox* box_hitbox, std::array<CollisionData, 8>& colision_data) = 0;
	
	HitBoxPrimitive(Vector3D* position, Quaternion* orientation, const Vector3D& position_offset);
public:
	bool doCollideWith(HitBoxPrimitive* hitbox, std::array<CollisionData, 8>& colision_data);
	virtual void update() = 0;
#ifdef HITBOX_DEBUG
	virtual void drawBox() = 0;
	virtual void drawCorner() = 0;
#endif // HITBOX_DEBUG

};

